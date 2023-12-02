#include <openssl/bio.h>
#include <openssl/evp.h>
#include "include/base64.h"

return_code_t base64_decode(
    char *encoded,
    size_t encoded_length,
    char *decoded
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
    if (decoded_length < 0) {
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    decoded[decoded_length] = '\0';
end:
    return return_code;
}
