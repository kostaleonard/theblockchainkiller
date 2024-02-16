/**
 * @brief Tests endian.c
 */

#ifndef TESTS_TEST_ENDIAN_H_
#define TESTS_TEST_ENDIAN_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_htobe64_correctly_encodes_data();

void test_betoh64_correctly_decodes_data();

#endif  // TESTS_TEST_ENDIAN_H_
