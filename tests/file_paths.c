#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "tests/file_paths.h"

int get_executable_directory(char *dirname) {
    int return_code = 0;
#ifdef _WIN32
    DWORD length = GetModuleFileName(NULL, dirname, TESTS_MAX_PATH);
    if (0 == length || length == TESTS_MAX_PATH) {
        return_code = 1;
        goto end;
    } else {
        // Remove the executable name to get the directory.
        char *last_backslash = strrchr(dirname, '\\');
        if (NULL != last_backslash) {
            *last_backslash = '\0';
        }
    }
#else
    // On Unix-like systems, read the symbolic link /proc/self/exe
    ssize_t length = readlink("/proc/self/exe", NULL, 0);
    if (length > 0) {
        if (readlink("/proc/self/exe", dirname, length + 1) == -1) {
            return_code = 2;
            goto end;
        } else {
            // Remove the executable name to get the directory.
            char *last_slash = strrchr(dirname, '/');
            if (NULL != last_slash) {
                *last_slash = '\0';
            }
        }
    }
#endif
end:
    return return_code;
}

int _get_tests_subdirectory(char *dirname, char *subdirname) {
    char executable_directory[TESTS_MAX_PATH] = {0};
    int return_code = get_executable_directory(executable_directory);
    if (0 != return_code) {
        goto end;
    }
    snprintf(
        dirname,
        TESTS_MAX_PATH,
        "%s/../tests/%s/",
        executable_directory,
        subdirname);
end:
    return return_code;
}

int get_fixture_directory(char *dirname) {
    return _get_tests_subdirectory(dirname, "fixtures");
}

int get_output_directory(char *dirname) {
    return _get_tests_subdirectory(dirname, "output");
}
