#include <stdio.h> //TODO remove debugging
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "include/base64.h"

return_code_t base64_decode(
    char *encoded,
    size_t encoded_length,
    char *decoded,
    size_t max_decoded_length
) {
    return_code_t return_code = SUCCESS;
    if (NULL == encoded || NULL == decoded) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    int decoded_length = EVP_DecodeBlock(
        (unsigned char *)decoded,
        (const unsigned char *)encoded,
        encoded_length
    );
    if (decoded_length <= 0) {
        // Error handling for decoding failure
        return_code = FAILURE_INVALID_INPUT; // TODO actual return code
        goto end;
    }
    decoded[decoded_length] = '\0'; // Null-terminate the decoded string
end:
    return return_code;
}
