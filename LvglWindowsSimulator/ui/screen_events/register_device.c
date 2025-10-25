#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <ui/cJSON.h>
#include "register_device.h"

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) return 0;
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

// --------------------- Helper: File & Directory ---------------------------
void ensure_dir(const char *dir) {
#ifdef _WIN32
    _mkdir(dir);
#else
    mkdir(dir, 0755);
#endif
}

char* read_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);
    return buf;
}

void write_file(const char *path, const char *data) {
    FILE *f = fopen(path, "w");
    if (!f) return;
    fputs(data, f);
    fclose(f);
}

// --------------------- JSON Handling ---------------------------
cJSON* load_device_status() {
    char *content = read_file(DEVICE_STATUS_FILE);
    if (!content) {
        // Default JSON if not found
        cJSON *status = cJSON_CreateObject();
        cJSON_AddBoolToObject(status, "deviceRegistered", 0);
        cJSON_AddStringToObject(status, "deviceId", "");
        cJSON_AddBoolToObject(status, "userRegistered", 0);
        char *json_str = cJSON_Print(status);
        write_file(DEVICE_STATUS_FILE, json_str);
        free(json_str);
        return status;
    }
    cJSON *json = cJSON_Parse(content);
    free(content);
    return json;
}

void save_device_status(cJSON *status) {
    char *out = cJSON_Print(status);
    write_file(DEVICE_STATUS_FILE, out);
    free(out);
}

// --------------------- HTTP Request ---------------------------
char* http_post_json(const char *url, const char *json_payload) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk = {0};

    curl = curl_easy_init();
    if (!curl) return NULL;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
        if (chunk.memory) free(chunk.memory);
        return NULL;
    }

    return chunk.memory; // caller must free()
}

// --------------------- Core Registration ---------------------------
void save_certificate_files(cJSON *data) {
    ensure_dir(CERTIFICATES_DIR);
    const cJSON *cert = cJSON_GetObjectItemCaseSensitive(data, "certificatePem");
    const cJSON *priv = cJSON_GetObjectItemCaseSensitive(data, "privateKey");
    const cJSON *pub  = cJSON_GetObjectItemCaseSensitive(data, "publicKey");

    if (cJSON_IsString(cert) && cert->valuestring)
        write_file(CERTIFICATES_DIR "/device_certificate.pem", cert->valuestring);
    if (cJSON_IsString(priv) && priv->valuestring)
        write_file(CERTIFICATES_DIR "/private_key.pem", priv->valuestring);
    if (cJSON_IsString(pub) && pub->valuestring)
        write_file(CERTIFICATES_DIR "/public_key.pem", pub->valuestring);
}

int check_register_device() {
    ensure_dir(CERTIFICATES_DIR);

    cJSON *status = load_device_status();
    cJSON *registered = cJSON_GetObjectItem(status, "deviceRegistered");
    if (cJSON_IsBool(registered) && registered->valueint) {
        printf("Device already registered ✅\n");
        cJSON_Delete(status);
        return 0;
    }

    printf("Sending registration request to server...\n");
    char *response = http_post_json(REGISTER_API_URL, "{}");
    if (!response) {
        printf("❌ Registration request failed.\n");
        cJSON_Delete(status);
        return 1;
    }

    cJSON *data = cJSON_Parse(response);
    free(response);
    if (!data) {
        printf("❌ Failed to parse server response.\n");
        cJSON_Delete(status);
        return 1;
    }

    cJSON *device_id = cJSON_GetObjectItemCaseSensitive(data, "deviceId");
    if (cJSON_IsString(device_id) && device_id->valuestring[0]) {
        cJSON_ReplaceItemInObject(status, "deviceRegistered", cJSON_CreateBool(1));
        cJSON_ReplaceItemInObject(status, "deviceId", cJSON_CreateString(device_id->valuestring));
        save_device_status(status);
        save_certificate_files(data);
        printf("Device registered successfully ✅. Device ID: %s\n", device_id->valuestring);

        // Register device with user server
        char payload[256];
        snprintf(payload, sizeof(payload), "{\"deviceId\":\"%s\"}", device_id->valuestring);
        char *user_resp = http_post_json(REGISTER_USER_API_URL, payload);
        if (user_resp) free(user_resp);
    } else {
        printf("❌ Registration failed: No deviceId received.\n");
    }

    cJSON_Delete(status);
    cJSON_Delete(data);
    return 0;
}
