#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "utils.h"

char* get_library_path(const char* subpath) {
    static char fullpath[256];
    char exe_path[256];

    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("[ERROR] readlink 실패");
        return NULL;
    }
    exe_path[len] = '\0';

    //  /home/veda/PoP/build-arm64
    char* last_slash = strrchr(exe_path, '/');
    if (last_slash) *last_slash = '\0';
    last_slash = strrchr(exe_path, '/');
    if (last_slash) *last_slash = '\0';

    if (snprintf(fullpath, sizeof(fullpath), "%s/%s", exe_path, subpath) >= sizeof(fullpath)) {
        fprintf(stderr, "[ERROR] get_library_path: 경로가 너무 깁니다.\n");
        return NULL;
    }

    return fullpath;
}
