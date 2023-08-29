#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "include/block.h"
#include "include/hash.h"
#include "include/transaction.h"
#include "include/return_codes.h"
#include "tests/test_block.h"

void test_block_create_gives_block() {
    linked_list_t *transaction_list = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list,
        (free_function_t *)transaction_destroy,
        (compare_function_t *)transaction_compare);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block = NULL;
    uint32_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block,
        transaction_list,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != block);
    block_destroy(block);
end:
}

void test_block_create_fails_on_invalid_input() {
    linked_list_t *transaction_list = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list,
        (free_function_t *)transaction_destroy,
        (compare_function_t *)transaction_compare);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block = NULL;
    uint32_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        NULL,
        transaction_list,
        proof_of_work,
        previous_block_hash);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = block_create(
        &block,
        NULL,
        proof_of_work,
        previous_block_hash);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    linked_list_destroy(transaction_list);
end:
}

void test_create_genesis_block_gives_block_with_genesis_values() {
    block_t *block = NULL;
    return_code_t return_code = block_create_genesis_block(&block);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    assert_true(NULL != block);
    bool is_empty = false;
    return_code = linked_list_is_empty(block->transaction_list, &is_empty);
    assert_true(SUCCESS == return_code);
    assert_true(is_empty);
    assert_true(GENESIS_BLOCK_PROOF_OF_WORK == block->proof_of_work);
    sha_256_t expected_previous_block_hash = {0};
    assert_true(0 == memcmp(
        &block->previous_block_hash,
        &expected_previous_block_hash,
        sizeof(sha_256_t)));
    block_destroy(block);
end:
}

void test_create_genesis_block_fails_on_invalid_input() {
    return_code_t return_code = block_create_genesis_block(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_block_destroy_returns_success() {
    linked_list_t *transaction_list = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list,
        (free_function_t *)transaction_destroy,
        (compare_function_t *)transaction_compare);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block = NULL;
    uint32_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block,
        transaction_list,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != block);
    return_code = block_destroy(block);
    assert_true(SUCCESS == return_code);
end:
}

void test_block_destroy_fails_on_invalid_input() {
    return_code_t return_code = block_destroy(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}
