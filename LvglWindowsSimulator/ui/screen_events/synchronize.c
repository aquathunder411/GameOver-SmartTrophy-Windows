#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include "synchronize.h"
#include "ui/cJSON.h"

#define DEVICE_ID "my_device_123"  // Replace with value loaded from file


static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (!mem->memory) return 0;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

// Check if file exists
int file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

// Get file size
long get_file_size(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    return st.st_size;
}

// Download file with libcurl
int download_file(const char *url, const char *output_path) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) return 1;

    fp = fopen(output_path, "wb");
    if (!fp) {
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);
    fclose(fp);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK) ? 0 : 1;
}

// Get Content-Length using HEAD
long get_remote_file_size(const char *url) {
    CURL *curl;
    double cl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) return -1;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return -1;
    }

    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK && cl > 0) ? (long)cl : -1;
}

/* Create directory if missing
void ensure_dir(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0755);
    }
}*/

// Main sync function
void run_sync_process() {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk = {0};

    ensure_dir(MEDIA_PATH);

    char api_url[512];
    snprintf(api_url, sizeof(api_url), "%s/%s", GET_MEDIA_API_URL, DEVICE_ID);
    printf("Syncing from API: %s\n", api_url);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl init failed\n");
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        return;
    }
    curl_easy_cleanup(curl);

    // Parse JSON
    cJSON *root = cJSON_Parse(chunk.memory);
    if (!root) {
        fprintf(stderr, "Invalid JSON\n");
        free(chunk.memory);
        return;
    }

    cJSON *success = cJSON_GetObjectItem(root, "success");
    if (!cJSON_IsTrue(success)) {
        fprintf(stderr, "API returned failure\n");
        cJSON_Delete(root);
        free(chunk.memory);
        return;
    }

    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (!cJSON_IsArray(data)) {
        fprintf(stderr, "No media list\n");
        cJSON_Delete(root);
        free(chunk.memory);
        return;
    }

    // Iterate over media items
    int media_count = cJSON_GetArraySize(data);
    for (int i = 0; i < media_count; i++) {
        cJSON *item = cJSON_GetArrayItem(data, i);
        const char *name = cJSON_GetObjectItem(item, "name")->valuestring;
        const char *media_type = cJSON_GetObjectItem(item, "media_type")->valuestring;
        const char *url = cJSON_GetObjectItem(item, "media_url")->valuestring;
        const char *created_at = cJSON_GetObjectItem(item, "createdAt")->valuestring;

        if (!name || !url || !media_type) continue;

        char safe_name[256];
        snprintf(safe_name, sizeof(safe_name), "%s_%s", name, created_at);

        char file_ext[16] = "bin";
        const char *dot = strrchr(url, '.');
        if (dot) strncpy(file_ext, dot + 1, sizeof(file_ext) - 1);

        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s/%s.%s", MEDIA_PATH, safe_name, file_ext);

        // Compare local vs remote size
        long local_size = file_exists(file_path) ? get_file_size(file_path) : -1;
        long remote_size = get_remote_file_size(url);

        if (local_size != remote_size || local_size < 0) {
            printf("Downloading: %s\n", file_path);
            if (download_file(url, file_path) == 0)
                printf("✓ Downloaded %s\n", file_path);
            else
                fprintf(stderr, "✗ Failed to download %s\n", file_path);
        } else {
            printf("No change: %s\n", file_path);
        }
    }

    // Cleanup
    cJSON_Delete(root);
    free(chunk.memory);

    printf("Synchronization complete.\n");
}
