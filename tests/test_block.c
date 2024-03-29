#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "include/base64.h"
#include "include/block.h"
#include "include/hash.h"
#include "include/transaction.h"
#include "include/return_codes.h"
#include "tests/test_block.h"
#include "tests/test_cryptography.h"

void test_block_create_gives_block() {
    linked_list_t *transaction_list = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list,
        (free_function_t *)transaction_destroy,
        NULL);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block = NULL;
    uint64_t proof_of_work = 123;
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
        NULL);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block = NULL;
    uint64_t proof_of_work = 123;
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
        NULL);
    if (SUCCESS != return_code) {
        assert_true(false);
        goto end;
    }
    block_t *block = NULL;
    uint64_t proof_of_work = 123;
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

void test_block_hash_gives_nonempty_hash() {
    linked_list_t *transaction_list = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block,
        transaction_list,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    sha_256_t hash = {0};
    return_code = block_hash(block, &hash);
    assert_true(SUCCESS == return_code);
    sha_256_t empty_hash = {0};
    assert_true(0 != memcmp(&hash, &empty_hash, sizeof(sha_256_t)));
    block_destroy(block);
}

void test_block_hash_same_fields_gives_same_hash() {
    // Block 1.
    linked_list_t *transaction_list1 = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list1,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Block 2.
    linked_list_t *transaction_list2 = NULL;
    return_code = linked_list_create(
        &transaction_list2,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block2 = NULL;
    return_code = block_create(
        &block2,
        transaction_list2,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Manually set created_at to match so that it doesn't affect hashing.
    block2->created_at = block1->created_at;
    sha_256_t hash1 = {0};
    return_code = block_hash(block1, &hash1);
    assert_true(SUCCESS == return_code);
    sha_256_t hash2 = {0};
    return_code = block_hash(block2, &hash2);
    assert_true(SUCCESS == return_code);
    assert_true(0 == memcmp(&hash1, &hash2, sizeof(sha_256_t)));
    block_destroy(block1);
    block_destroy(block2);
}

void test_block_hash_created_at_included_in_hash() {
    // Block 1.
    linked_list_t *transaction_list1 = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list1,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Block 2.
    linked_list_t *transaction_list2 = NULL;
    return_code = linked_list_create(
        &transaction_list2,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block2 = NULL;
    return_code = block_create(
        &block2,
        transaction_list2,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // block_create fills in created_at with the current time. However, the time
    // granularity is not very fine and sometimes block1 and block2 get the same
    // created_at value. We manually increment block2->created_at to make sure
    // they differ.
    block2->created_at = block1->created_at + 1;
    sha_256_t hash1 = {0};
    return_code = block_hash(block1, &hash1);
    assert_true(SUCCESS == return_code);
    sha_256_t hash2 = {0};
    return_code = block_hash(block2, &hash2);
    assert_true(SUCCESS == return_code);
    assert_true(0 != memcmp(&hash1, &hash2, sizeof(sha_256_t)));
    block_destroy(block1);
    block_destroy(block2);
}

void test_block_hash_transactions_included_in_hash() {
    // Block 1.
    linked_list_t *transaction_list1 = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list1,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    transaction_t *transaction = NULL;
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t sender_public_key = {0};
    return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t sender_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        sender_private_key.bytes);
    // In these test transactions, the sender and recipient are the same.
    ssh_key_t recipient_public_key = {0};
    memcpy(
        &recipient_public_key,
        &sender_public_key,
        sizeof(recipient_public_key));
    uint64_t amount = 5;
    return_code = transaction_create(
        &transaction,
        &sender_public_key,
        &recipient_public_key,
        amount,
        &sender_private_key);
    assert_true(SUCCESS == return_code);
    return_code = linked_list_prepend(transaction_list1, transaction);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Block 2.
    linked_list_t *transaction_list2 = NULL;
    return_code = linked_list_create(
        &transaction_list2,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block2 = NULL;
    return_code = block_create(
        &block2,
        transaction_list2,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Manually set created_at to match so that it doesn't affect hashing.
    block2->created_at = block1->created_at;
    sha_256_t hash1 = {0};
    return_code = block_hash(block1, &hash1);
    assert_true(SUCCESS == return_code);
    sha_256_t hash2 = {0};
    return_code = block_hash(block2, &hash2);
    assert_true(SUCCESS == return_code);
    assert_true(0 != memcmp(&hash1, &hash2, sizeof(sha_256_t)));
    block_destroy(block1);
    block_destroy(block2);
}

void test_block_hash_multiple_transactions_included_in_hash() {
    // Block 1.
    linked_list_t *transaction_list1 = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list1,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    transaction_t *transaction1 = NULL;
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t user_1_public_key = {0};
    return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        user_1_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t user_1_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        user_1_private_key.bytes);
    // In these test transactions, the sender and recipient are the same.
    ssh_key_t user_2_public_key = {0};
    memcpy(
        &user_2_public_key,
        &user_1_public_key,
        sizeof(user_2_public_key));
    ssh_key_t user_2_private_key = {0};
    memcpy(
        &user_2_private_key,
        &user_1_private_key,
        sizeof(user_2_private_key));
    uint64_t amount = 5;
    return_code = transaction_create(
        &transaction1,
        &user_1_public_key,
        &user_2_public_key,
        amount,
        &user_1_private_key);
    assert_true(SUCCESS == return_code);
    // transaction2 is a copy of transaction1. We need to use a different memory
    // location to protect against race conditions when destroying both blocks.
    transaction_t *transaction2 = malloc(sizeof(transaction_t));
    memcpy(transaction2, transaction1, sizeof(transaction_t));
    return_code = linked_list_prepend(transaction_list1, transaction1);
    assert_true(SUCCESS == return_code);
    transaction_t *transaction3 = NULL;
    uint64_t amount2 = 17;
    return_code = transaction_create(
        &transaction3,
        &user_2_public_key,
        &user_1_public_key,
        amount2,
        &user_2_private_key);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Block 2.
    linked_list_t *transaction_list2 = NULL;
    return_code = linked_list_create(
        &transaction_list2,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block2 = NULL;
    return_code = block_create(
        &block2,
        transaction_list2,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Manually set created_at to match so that it doesn't affect hashing.
    block2->created_at = block1->created_at;
    return_code = linked_list_prepend(transaction_list2, transaction2);
    assert_true(SUCCESS == return_code);
    sha_256_t hash1 = {0};
    return_code = block_hash(block1, &hash1);
    assert_true(SUCCESS == return_code);
    sha_256_t hash2 = {0};
    return_code = block_hash(block2, &hash2);
    assert_true(SUCCESS == return_code);
    assert_true(0 == memcmp(&hash1, &hash2, sizeof(sha_256_t)));
    // Now add a second transaction to block 1 and make sure the hash changes.
    return_code = linked_list_prepend(transaction_list1, transaction3);
    assert_true(SUCCESS == return_code);
    return_code = block_hash(block1, &hash1);
    assert_true(SUCCESS == return_code);
    assert_true(0 != memcmp(&hash1, &hash2, sizeof(sha_256_t)));
    block_destroy(block1);
    block_destroy(block2);
}

void test_block_hash_proof_of_work_included_in_hash() {
    // Block 1.
    linked_list_t *transaction_list1 = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list1,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Block 2.
    linked_list_t *transaction_list2 = NULL;
    return_code = linked_list_create(
        &transaction_list2,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block2 = NULL;
    return_code = block_create(
        &block2,
        transaction_list2,
        proof_of_work + 1,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    // Manually set created_at to match so that it doesn't affect hashing.
    block2->created_at = block1->created_at;
    sha_256_t hash1 = {0};
    return_code = block_hash(block1, &hash1);
    assert_true(SUCCESS == return_code);
    sha_256_t hash2 = {0};
    return_code = block_hash(block2, &hash2);
    assert_true(SUCCESS == return_code);
    assert_true(0 != memcmp(&hash1, &hash2, sizeof(sha_256_t)));
    block_destroy(block1);
    block_destroy(block2);
}

void test_block_hash_previous_block_hash_included_in_hash() {
    // Block 1.
    linked_list_t *transaction_list1 = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list1,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block1 = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash1 = {0};
    return_code = block_create(
        &block1,
        transaction_list1,
        proof_of_work,
        previous_block_hash1);
    assert_true(SUCCESS == return_code);
    // Block 2.
    linked_list_t *transaction_list2 = NULL;
    return_code = linked_list_create(
        &transaction_list2,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block2 = NULL;
    sha_256_t previous_block_hash2 = {0};
    previous_block_hash2.digest[0] = 'A';
    return_code = block_create(
        &block2,
        transaction_list2,
        proof_of_work,
        previous_block_hash2);
    assert_true(SUCCESS == return_code);
    // Manually set created_at to match so that it doesn't affect hashing.
    block2->created_at = block1->created_at;
    sha_256_t hash1 = {0};
    return_code = block_hash(block1, &hash1);
    assert_true(SUCCESS == return_code);
    sha_256_t hash2 = {0};
    return_code = block_hash(block2, &hash2);
    assert_true(SUCCESS == return_code);
    assert_true(0 != memcmp(&hash1, &hash2, sizeof(sha_256_t)));
    block_destroy(block1);
    block_destroy(block2);
}

void test_block_hash_fails_on_invalid_input() {
    linked_list_t *transaction_list = NULL;
    return_code_t return_code = linked_list_create(
        &transaction_list,
        (free_function_t *)transaction_destroy,
        NULL);
    assert_true(SUCCESS == return_code);
    block_t *block = NULL;
    uint64_t proof_of_work = 123;
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        &block,
        transaction_list,
        proof_of_work,
        previous_block_hash);
    assert_true(SUCCESS == return_code);
    sha_256_t hash = {0};
    return_code = block_hash(NULL, &hash);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = block_hash(block, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    block_destroy(block);
}
