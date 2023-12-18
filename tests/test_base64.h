/**
 * @brief Tests base64.c
 */

#ifndef TESTS_TEST_BASE64_H_
#define TESTS_TEST_BASE64_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_base64_decode_correctly_decodes();

void test_base64_decode_fails_on_invalid_input();

#endif  // TESTS_TEST_BASE64_H_
