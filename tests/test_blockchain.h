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

void test_blockchain_destroy_returns_success();

void test_blockchain_destroy_fails_on_invalid_input();

void test_blockchain_add_block_appends_block();

void test_blockchain_add_block_fails_on_invalid_input();

void test_blockchain_is_valid_block_hash_true_on_valid_hash();

void test_blockchain_is_valid_block_hash_false_on_invalid_hash();

void test_blockchain_is_valid_block_hash_fails_on_invalid_input();

void test_blockchain_mine_block_produces_block_with_valid_hash();

void test_blockchain_mine_block_fails_on_invalid_input();

#endif  // TESTS_TEST_BLOCKCHAIN_H_
