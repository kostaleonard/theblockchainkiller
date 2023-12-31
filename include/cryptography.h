/**
 * @brief Defines cryptography types and functions.
 */

#ifndef INCLUDE_CRYPTOGRAPHY_H_
#define INCLUDE_CRYPTOGRAPHY_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/return_codes.h"

#define MAX_SSH_KEY_LENGTH 4096
// OpenSSL digital signatures are 1/8 the size of the key length.
#define MAX_SSH_SIGNATURE_LENGTH 512

/**
 * @brief Contains an SSH key.
 * 
 * @param bytes The SSH key string contents.
 */
typedef struct ssh_key_t {
    char bytes[MAX_SSH_KEY_LENGTH];
} ssh_key_t;

/**
 * @brief Contains an SSH signature.
 * 
 * @param bytes The SSH signature string contents.
 */
typedef struct ssh_signature_t {
    size_t length;
    unsigned char bytes[MAX_SSH_SIGNATURE_LENGTH];
} ssh_signature_t;

#endif  // INCLUDE_CRYPTOGRAPHY_H_
