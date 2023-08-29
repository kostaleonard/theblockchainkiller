/**
 * @brief Tests blockchain.c
 */

#ifndef TESTS_TEST_BLOCKCHAIN_H_
#define TESTS_TEST_BLOCKCHAIN_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_blockchain_create_gives_blockchain();

void test_blockchain_create_fails_on_invalid_input();

#endif  // TESTS_TEST_BLOCKCHAIN_H_
