#ifndef SYNCHRONIZE_H
#define SYNCHRONIZE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   // for size_t
#include "register_device.h" // For MemoryStruct

// -----------------------------------------------------------------------------
// Configuration constants
// -----------------------------------------------------------------------------

#define MEDIA_PATH "./assets"
#define GET_MEDIA_API_URL "http://52.66.24.161:8000/api/iot/media"

// -----------------------------------------------------------------------------
// Function declarations
// -----------------------------------------------------------------------------

/**
 * @brief Perform the synchronization process.
 *        Fetches media metadata from the API and downloads/updates files.
 */
void run_sync_process(void);

/**
 * @brief Ensures the directory exists (creates it if missing).
 * @param path Path to the directory.
 */
void ensure_dir(const char *path);

/**
 * @brief Check if a file exists on the local system.
 * @param path File path to check.
 * @return 1 if exists, 0 otherwise.
 */
int file_exists(const char *path);

/**
 * @brief Get the size of a local file.
 * @param path File path.
 * @return File size in bytes, or -1 if not found.
 */
long get_file_size(const char *path);

/**
 * @brief Download a file from a given URL to a local output path.
 * @param url Remote file URL.
 * @param output_path Local file path where to save.
 * @return 0 on success, 1 on failure.
 */
int download_file(const char *url, const char *output_path);

/**
 * @brief Retrieve the remote file size via HTTP HEAD request.
 * @param url Remote file URL.
 * @return File size in bytes, or -1 on error.
 */
long get_remote_file_size(const char *url);

#ifdef __cplusplus
}
#endif

#endif // SYNCHRONIZE_H