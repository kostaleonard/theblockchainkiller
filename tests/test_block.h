/**
 * @brief Tests block.c
 */

#ifndef TESTS_TEST_BLOCK_H_
#define TESTS_TEST_BLOCK_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_block_create_gives_block();

void test_block_create_fails_on_invalid_input();

void test_block_destroy_returns_success();

void test_block_destroy_fails_on_invalid_input();

#endif  // TESTS_TEST_BLOCK_H_
