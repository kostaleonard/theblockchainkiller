#include <stdbool.h>
#include <stdlib.h>
#include "include/block.h"
#include "include/blockchain.h"
#include "include/hash.h"
#include "include/linked_list.h"
#include "tests/test_blockchain.h"

#define NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH 2
#define EXPERIMENTALLY_FOUND_PROOF_OF_WORK 90797

void test_blockchain_create_gives_blockchain() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != blockchain);
    assert_true(NULL != blockchain->block_list);
    assert_true(NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH ==
        blockchain->num_leading_zero_bytes_required_in_block_hash);
    blockchain_destroy(blockchain);
}

void test_blockchain_create_fails_on_invalid_input() {
    return_code_t return_code = blockchain_create(
        NULL, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_blockchain_destroy_returns_success() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    return_code = blockchain_destroy(blockchain);
    assert_true(SUCCESS == return_code);
end:
}

void test_blockchain_destroy_fails_on_invalid_input() {
    return_code_t return_code = blockchain_destroy(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_blockchain_add_block_appends_block() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    return_code = blockchain_add_block(blockchain, genesis_block);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    linked_list_t *transaction_list1 = NULL;
    return_code = linked_list_create(&transaction_list1, free, NULL);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block1 = NULL;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        123,
        previous_block_hash);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    return_code = blockchain_add_block(blockchain, block1);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    linked_list_t *transaction_list2 = NULL;
    return_code = linked_list_create(&transaction_list2, free, NULL);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block2 = NULL;
    return_code = block_create(
        &block2,
        transaction_list2,
        456,
        previous_block_hash);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    return_code = blockchain_add_block(blockchain, block2);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *first_block = (block_t *)blockchain->block_list->head->data;
    block_t *second_block = (block_t *)blockchain->block_list->head->next->data;
    block_t *third_block =
        (block_t *)blockchain->block_list->head->next->next->data;
    assert_true(GENESIS_BLOCK_PROOF_OF_WORK == first_block->proof_of_work);
    assert_true(123 == second_block->proof_of_work);
    assert_true(456 == third_block->proof_of_work);
end:
    blockchain_destroy(blockchain);
}

void test_blockchain_add_block_fails_on_invalid_input() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    return_code = blockchain_add_block(NULL, genesis_block);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_add_block(blockchain, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    block_destroy(genesis_block);
end:
    blockchain_destroy(blockchain);
}

void test_blockchain_is_valid_block_hash_true_on_valid_hash() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    sha_256_t hash = {0};
    bool is_valid_block_hash = false;
    return_code = blockchain_is_valid_block_hash(
        blockchain, hash, &is_valid_block_hash);
    assert_true(SUCCESS == return_code);
    assert_true(is_valid_block_hash);
    // Set a non-leading zero to a nonzero value.
    // This change should not affect the output.
    hash.digest[NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH] = 9;
    return_code = blockchain_is_valid_block_hash(
        blockchain, hash, &is_valid_block_hash);
    assert_true(SUCCESS == return_code);
    assert_true(is_valid_block_hash);
    blockchain_destroy(blockchain);
}

void test_blockchain_is_valid_block_hash_false_on_invalid_hash() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    sha_256_t hash = {0};
    // Set a leading zero to a nonzero value.
    hash.digest[1] = 9;
    bool is_valid_block_hash = false;
    return_code = blockchain_is_valid_block_hash(
        blockchain, hash, &is_valid_block_hash);
    assert_true(SUCCESS == return_code);
    assert_true(!is_valid_block_hash);
    blockchain_destroy(blockchain);
}

void test_blockchain_is_valid_block_hash_fails_on_invalid_input() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    sha_256_t hash = {0};
    bool is_valid_block_hash = false;
    return_code = blockchain_is_valid_block_hash(
        NULL, hash, &is_valid_block_hash);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_is_valid_block_hash(
        blockchain, hash, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    blockchain_destroy(blockchain);
}

void test_blockchain_mine_block_produces_block_with_valid_hash() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    linked_list_t *transaction_list1 = NULL;
    return_code = linked_list_create(&transaction_list1, free, NULL);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        0,
        previous_block_hash);
    // Manually set created_at to get a consistent hash.
    block1->created_at = 0;
    return_code = blockchain_mine_block(blockchain, block1, false);
    assert_true(SUCCESS == return_code);
    assert_true(0 != block1->proof_of_work);
    assert_true(EXPERIMENTALLY_FOUND_PROOF_OF_WORK == block1->proof_of_work);
    sha_256_t hash = {0};
    return_code = block_hash(block1, &hash);
    assert_true(SUCCESS == return_code);
    bool is_valid_block_hash = false;
    return_code = blockchain_is_valid_block_hash(
        blockchain, hash, &is_valid_block_hash);
    assert_true(SUCCESS == return_code);
    assert_true(is_valid_block_hash);
    block_destroy(block1);
    blockchain_destroy(blockchain);
}

void test_blockchain_mine_block_fails_on_invalid_input() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    linked_list_t *transaction_list1 = NULL;
    return_code = linked_list_create(&transaction_list1, free, NULL);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        0,
        previous_block_hash);
    return_code = blockchain_mine_block(NULL, block1, false);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_mine_block(blockchain, NULL, false);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    block_destroy(block1);
    blockchain_destroy(blockchain);
}
