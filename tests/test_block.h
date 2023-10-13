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

void test_create_genesis_block_gives_block_with_genesis_values();

void test_create_genesis_block_fails_on_invalid_input();

void test_block_destroy_returns_success();

void test_block_destroy_fails_on_invalid_input();

void test_block_hash_gives_nonempty_hash();

void test_block_hash_same_fields_gives_same_hash();

void test_block_hash_created_at_included_in_hash();

void test_block_hash_transactions_included_in_hash();

void test_block_hash_proof_of_work_included_in_hash();

void test_block_hash_previous_block_hash_included_in_hash();

void test_block_hash_fails_on_invalid_input();

#endif  // TESTS_TEST_BLOCK_H_
