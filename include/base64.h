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
 * @param decoded The pointer to fill with the base 64 decoded string.
 * @param max_decoded_length The maximum number of characters to write to the
 * output pointer.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t base64_decode(
    char *encoded,
    size_t encoded_length,
    char *decoded,
    size_t max_decoded_length
);

#endif  // INCLUDE_BASE64_H_
