/**
 * @brief Defines cryptography types and functions.
 */

#ifndef INCLUDE_CRYPTOGRAPHY_H_
#define INCLUDE_CRYPTOGRAPHY_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/return_codes.h"

#define MAX_SSH_KEY_LENGTH 1024

/**
 * @brief Contains an SSH key.
 * 
 * @param bytes The SSH key string contents.
 */
typedef struct ssh_key_t {
    char bytes[MAX_SSH_KEY_LENGTH];
} ssh_key_t;

#endif  // INCLUDE_CRYPTOGRAPHY_H_
