#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int get_executable_directory(char *dirname) {
    int return_code = 0;
#ifdef _WIN32
    DWORD length = GetModuleFileName(NULL, dirname, MAX_PATH);
    if (0 == length || length == MAX_PATH) {
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

int get_fixture_directory(char *dirname) {
    // TODO MAX_PATH is only on Windows?
    char executable_directory[MAX_PATH] = {0};
    int return_code = get_executable_directory(executable_directory);
    if (0 != return_code) {
        goto end;
    }
    printf("Executable directory: %s\n", executable_directory); //TODO remove
    snprintf(
        dirname,
        MAX_PATH,
        "%s/../tests/fixtures/",
        executable_directory);
end:
    return return_code;
}

int get_output_directory(char *dirname) {
    // TODO MAX_PATH is only on Windows?
    char executable_directory[MAX_PATH] = {0};
    int return_code = get_executable_directory(executable_directory);
    if (0 != return_code) {
        goto end;
    }
    snprintf(
        dirname,
        MAX_PATH,
        "%s/../tests/output/",
        executable_directory);
end:
    return return_code;
}
