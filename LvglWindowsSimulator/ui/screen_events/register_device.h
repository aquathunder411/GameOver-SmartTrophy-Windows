#ifndef REGISTER_DEVICE_H
#define REGISTER_DEVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <ui/cJSON.h>

// --------------------- Constants -----------------------
#define DEVICE_STATUS_FILE "device_status.json"
#define CERTIFICATES_DIR   "certificates"
#define REGISTER_API_URL   "http://52.66.24.161:5000/registerDevice"
#define REGISTER_USER_API_URL "http://52.66.24.161:8000/api/iot/device/add"

// --------------------- Data Structures ------------------
struct MemoryStruct {
    char *memory;
    size_t size;
};

// --------------------- Function Declarations ------------

// ---- File & Directory Utilities ----
void ensure_dir(const char *dir);
char* read_file(const char *path);
void write_file(const char *path, const char *data);

// ---- JSON Handling ----
cJSON* load_device_status(void);
void save_device_status(cJSON *status);

// ---- HTTP Communication ----
char* http_post_json(const char *url, const char *json_payload);

// ---- Certificate Management ----
void save_certificate_files(cJSON *data);

// ---- Device Registration ----
int check_register_device(void);

// ---- Curl Write Callback ----
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif // REGISTER_DEVICE_H