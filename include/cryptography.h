/**
 * @brief Defines cryptography types and functions.
 */

#ifndef INCLUDE_CRYPTOGRAPHY_H_
#define INCLUDE_CRYPTOGRAPHY_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/return_codes.h"

// TODO separate key sizes for public and private keys, since private keys are much (4x) larger
#define MAX_SSH_KEY_LENGTH 4096
// TODO how big is an SSH signature?
#define MAX_SSH_SIGNATURE_LENGTH 1024

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
    char bytes[MAX_SSH_SIGNATURE_LENGTH];
} ssh_signature_t;

#endif  // INCLUDE_CRYPTOGRAPHY_H_
