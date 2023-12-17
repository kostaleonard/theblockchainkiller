#include <string.h>
#include "include/base64.h"
#include "include/return_codes.h"
#include "tests/test_base64.h"

#define TEST_STRING_1 "Hello base64!\n"
#define TEST_STRING_1_BASE64_ENCODED "SGVsbG8gYmFzZTY0IQo="
#define TEST_STRING_2 "Hello\nbase64\n"
#define TEST_STRING_2_BASE64_ENCODED "SGVsbG8KYmFzZTY0Cg=="
#define TEST_MAX_DECODED_LENGTH 1024

void test_base64_decode_correctly_decodes() {
    char decoded[TEST_MAX_DECODED_LENGTH];
    return_code_t return_code = base64_decode(
        TEST_STRING_1_BASE64_ENCODED,
        strlen(TEST_STRING_1_BASE64_ENCODED),
        decoded);
    assert_true(SUCCESS == return_code);
    assert_true(0 == strncmp(decoded, TEST_STRING_1, TEST_MAX_DECODED_LENGTH));
    return_code = base64_decode(
        TEST_STRING_2_BASE64_ENCODED,
        strlen(TEST_STRING_2_BASE64_ENCODED),
        decoded);
    assert_true(SUCCESS == return_code);
    assert_true(0 == strncmp(decoded, TEST_STRING_2, TEST_MAX_DECODED_LENGTH));
}

void test_base64_decode_fails_on_invalid_input() {
    char decoded[TEST_MAX_DECODED_LENGTH];
    return_code_t return_code = base64_decode(
        NULL,
        strlen(TEST_STRING_1_BASE64_ENCODED),
        decoded);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = base64_decode(
        TEST_STRING_1_BASE64_ENCODED,
        strlen(TEST_STRING_1_BASE64_ENCODED),
        NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}
