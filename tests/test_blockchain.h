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

void test_blockchain_verify_succeeds_on_valid_blockchain();

void test_blockchain_verify_fails_on_invalid_genesis_block();

void test_blockchain_verify_fails_on_invalid_proof_of_work();

void test_blockchain_verify_fails_on_invalid_previous_block_hash();

void test_blockchain_verify_fails_on_invalid_transaction_signature();

void test_blockchain_verify_fails_on_invalid_input();

void test_blockchain_serialize_creates_nonempty_buffer();

void test_blockchain_serialize_fails_on_invalid_input();

void test_blockchain_deserialize_reconstructs_blockchain();

void test_blockchain_deserialize_fails_on_attempted_read_past_buffer();

void test_blockchain_deserialize_fails_on_invalid_input();

void test_blockchain_write_to_file_creates_nonempty_file();

void test_blockchain_write_to_file_fails_on_invalid_input();

void test_blockchain_read_from_file_reconstructs_blockchain();

void test_blockchain_read_from_file_fails_on_invalid_input();

#endif  // TESTS_TEST_BLOCKCHAIN_H_
