/**
 * @brief Defines functions for working with base 64 encoding.
 */

#ifndef INCLUDE_BASE64_H_
#define INCLUDE_BASE64_H_

#include <stdint.h>
#include "include/return_codes.h"

/**
 * @brief Fills decoded with the result of base 64 decoding encoded.
 * 
 * @param encoded The base 64 encoded string.
 * @param encoded_length The length of the encoded string.
 * @param decoded The pointer to fill with the base 64 decoded string. This
 * buffer needs to be at least 3/4 the size of the encoded string (base 64
 * strings are always a multiple of 4 in length).
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t base64_decode(
    char *encoded,
    size_t encoded_length,
    char *decoded
);

#endif  // INCLUDE_BASE64_H_
