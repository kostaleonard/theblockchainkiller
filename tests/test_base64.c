#include <string.h>
#include "include/base64.h"
#include "include/return_codes.h"
#include "tests/test_base64.h"

#define TEST_STRING_1 "Hello base64!\n"
#define TEST_STRING_1_BASE64_ENCODED "SGVsbG8gYmFzZTY0IQo="
#define TEST_STRING_2 "Hello\nbase64\n"
#define TEST_STRING_2_BASE64_ENCODED "SGVsbG8KYmFzZTY0Cg=="

void test_base64_decode_correctly_decodes() {
    size_t max_decoded_length = 1024;
    char decoded[max_decoded_length];
    return_code_t return_code = base64_decode(
        TEST_STRING_1_BASE64_ENCODED,
        strlen(TEST_STRING_1_BASE64_ENCODED),
        decoded,
        max_decoded_length);
    assert_true(SUCCESS == return_code);
    assert_true(0 == strncmp(decoded, TEST_STRING_1, max_decoded_length));
    return_code = base64_decode(
        TEST_STRING_2_BASE64_ENCODED,
        strlen(TEST_STRING_2_BASE64_ENCODED),
        decoded,
        max_decoded_length);
    assert_true(SUCCESS == return_code);
    assert_true(0 == strncmp(decoded, TEST_STRING_2, max_decoded_length));
}

void test_base64_decode_fails_on_invalid_input() {
    size_t max_decoded_length = 1024;
    char decoded[max_decoded_length];
    return_code_t return_code = base64_decode(
        NULL,
        strlen(TEST_STRING_1_BASE64_ENCODED),
        decoded,
        max_decoded_length);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = base64_decode(
        TEST_STRING_1_BASE64_ENCODED,
        strlen(TEST_STRING_1_BASE64_ENCODED),
        NULL,
        max_decoded_length);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}
