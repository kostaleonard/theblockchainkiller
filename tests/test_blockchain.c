#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "include/block.h"
#include "include/blockchain.h"
#include "include/hash.h"
#include "include/linked_list.h"
#include "include/transaction.h"
#include "tests/file_paths.h"
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

void test_blockchain_verify_succeeds_on_valid_blockchain() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(&blockchain, infile);
    assert_true(SUCCESS == return_code);
    bool is_valid = false;
    block_t *first_invalid_block = NULL;
    return_code = blockchain_verify(
        blockchain, &is_valid, &first_invalid_block);
    assert_true(SUCCESS == return_code);
    assert_true(is_valid);
    assert_true(NULL == first_invalid_block);
    blockchain_destroy(blockchain);
}

void test_blockchain_verify_fails_on_invalid_genesis_block() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(&blockchain, infile);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = (block_t *)blockchain->block_list->head->data;
    genesis_block->proof_of_work += 1;
    bool is_valid = false;
    block_t *first_invalid_block = NULL;
    return_code = blockchain_verify(
        blockchain, &is_valid, &first_invalid_block);
    assert_true(SUCCESS == return_code);
    assert_true(!is_valid);
    assert_true(first_invalid_block == genesis_block);
    genesis_block->proof_of_work = GENESIS_BLOCK_PROOF_OF_WORK;
    genesis_block->previous_block_hash.digest[0] = 'A';
    is_valid = false;
    first_invalid_block = NULL;
    return_code = blockchain_verify(
        blockchain, &is_valid, &first_invalid_block);
    assert_true(SUCCESS == return_code);
    assert_true(!is_valid);
    assert_true(first_invalid_block == genesis_block);
    blockchain_destroy(blockchain);
}

void test_blockchain_verify_fails_on_invalid_proof_of_work() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(&blockchain, infile);
    assert_true(SUCCESS == return_code);
    block_t *block = (block_t *)blockchain->block_list->head->next->data;
    block->proof_of_work += 1;
    bool is_valid = false;
    block_t *first_invalid_block = NULL;
    return_code = blockchain_verify(
        blockchain, &is_valid, &first_invalid_block);
    assert_true(SUCCESS == return_code);
    assert_true(!is_valid);
    assert_true(first_invalid_block == block);
    blockchain_destroy(blockchain);
}

void test_blockchain_verify_fails_on_invalid_previous_block_hash() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(&blockchain, infile);
    assert_true(SUCCESS == return_code);
    block_t *block = (block_t *)blockchain->block_list->head->next->next->data;
    block->previous_block_hash.digest[0] = 'A';
    block->previous_block_hash.digest[1] = 'A';
    block->previous_block_hash.digest[2] = 'A';
    bool is_valid = false;
    block_t *first_invalid_block = NULL;
    return_code = blockchain_verify(
        blockchain, &is_valid, &first_invalid_block);
    assert_true(SUCCESS == return_code);
    assert_true(!is_valid);
    assert_true(first_invalid_block == block);
    blockchain_destroy(blockchain);
}

void test_blockchain_verify_fails_on_invalid_transaction_signature() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(&blockchain, infile);
    assert_true(SUCCESS == return_code);
    block_t *block = (block_t *)blockchain->block_list->head->next->next->data;
    transaction_t *minting_transaction =
        (transaction_t *)block->transaction_list->head->data;
    minting_transaction->sender_signature.bytes[0] = 'A';
    minting_transaction->sender_signature.bytes[1] = 'A';
    minting_transaction->sender_signature.bytes[2] = 'A';
    bool is_valid = false;
    block_t *first_invalid_block = NULL;
    return_code = blockchain_verify(
        blockchain, &is_valid, &first_invalid_block);
    assert_true(SUCCESS == return_code);
    assert_true(!is_valid);
    assert_true(first_invalid_block == block);
    blockchain_destroy(blockchain);
}

void test_blockchain_verify_fails_on_invalid_input() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(&blockchain, infile);
    assert_true(SUCCESS == return_code);
    bool is_valid = false;
    block_t *first_invalid_block = NULL;
    return_code = blockchain_verify(
        NULL, &is_valid, &first_invalid_block);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_verify(
        blockchain, NULL, &first_invalid_block);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    blockchain_destroy(blockchain);
}

void test_blockchain_serialize_creates_nonempty_buffer() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = blockchain_serialize(blockchain, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != buffer);
    assert_true(0 != buffer_size);
    unsigned char *empty_buffer = calloc(buffer_size, 1);
    assert_true(0 != memcmp(buffer, empty_buffer, buffer_size));
    free(buffer);
    free(empty_buffer);
    blockchain_destroy(blockchain);
}

void test_blockchain_serialize_fails_on_invalid_input() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = blockchain_serialize(NULL, &buffer, &buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_serialize(blockchain, NULL, &buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_serialize(blockchain, &buffer, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    blockchain_destroy(blockchain);
}

void test_blockchain_deserialize_reconstructs_blockchain() {
    // See test_blockchain_read_from_file_reconstructs_blockchain. Since
    // blockchain_read_from_file calls blockchain_deserialize, the
    // aforementioned test already covers deserialization fairly well. Here we
    // will just test on a single-block blockchain to preserve compute and add
    // diversity to the tests.
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = blockchain_serialize(blockchain, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    blockchain_t *deserialized_blockchain = NULL;
    return_code = blockchain_deserialize(
        &deserialized_blockchain, buffer, buffer_size);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != deserialized_blockchain);
    uint64_t num_blocks = 0;
    return_code = linked_list_length(
        deserialized_blockchain->block_list, &num_blocks);
    assert_true(SUCCESS == return_code);
    assert_true(1 == num_blocks);
    block_t *deserialized_genesis_block =
        (block_t *)deserialized_blockchain->block_list->head->data;
    assert_true(
        genesis_block->created_at == deserialized_genesis_block->created_at);
    assert_true(0 == memcmp(
        &genesis_block->previous_block_hash,
        &deserialized_genesis_block->previous_block_hash,
        sizeof(sha_256_t)));
    assert_true(
        genesis_block->proof_of_work ==
        deserialized_genesis_block->proof_of_work);
    bool is_empty = false;
    return_code = linked_list_is_empty(
        deserialized_genesis_block->transaction_list, &is_empty);
    assert_true(SUCCESS == return_code);
    assert_true(is_empty);
    free(buffer);
    blockchain_destroy(blockchain);
    blockchain_destroy(deserialized_blockchain);
}

void test_blockchain_deserialize_fails_on_attempted_read_past_buffer() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = blockchain_serialize(blockchain, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    blockchain_t *deserialized_blockchain = NULL;
    return_code = blockchain_deserialize(
        &deserialized_blockchain, buffer, buffer_size - 50);
    assert_true(FAILURE_BUFFER_TOO_SMALL == return_code);
    free(buffer);
    blockchain_destroy(blockchain);
}

void test_blockchain_deserialize_fails_on_invalid_input() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = blockchain_serialize(blockchain, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    blockchain_t *deserialized_blockchain = NULL;
    return_code = blockchain_deserialize(NULL, buffer, buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_deserialize(
        &deserialized_blockchain, NULL, buffer_size);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    free(buffer);
    blockchain_destroy(blockchain);
}

void test_blockchain_write_to_file_creates_nonempty_file() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    char output_directory[TESTS_MAX_PATH];
    get_output_directory(output_directory);
    char outfile[TESTS_MAX_PATH];
    int return_value = snprintf(
        outfile,
        TESTS_MAX_PATH,
        "%s/%s",
        output_directory,
        "blockchain_test_blockchain_write_to_file_creates_nonempty_file");
    assert_true(return_value < TESTS_MAX_PATH);
    struct stat file_stats = {0};
    assert_true(0 != stat(outfile, &file_stats));
    return_code = blockchain_write_to_file(blockchain, outfile);
    assert_true(SUCCESS == return_code);
    assert_true(0 == stat(outfile, &file_stats));
    assert_true(0 != file_stats.st_size);
    blockchain_destroy(blockchain);
}

void test_blockchain_write_to_file_fails_on_invalid_input() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    char output_directory[TESTS_MAX_PATH];
    get_output_directory(output_directory);
    char outfile[TESTS_MAX_PATH];
    int return_value = snprintf(
        outfile,
        TESTS_MAX_PATH,
        "%s/%s",
        output_directory,
        "blockchain_test_blockchain_write_to_file_fails_on_invalid_input");
    assert_true(return_value < TESTS_MAX_PATH);
    return_code = blockchain_write_to_file(blockchain, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_write_to_file(NULL, outfile);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    blockchain_destroy(blockchain);
}

void test_blockchain_read_from_file_reconstructs_blockchain() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(&blockchain, infile);
    assert_true(SUCCESS == return_code);
    assert_true(NULL != blockchain);
    assert_true(blockchain->num_leading_zero_bytes_required_in_block_hash != 0);
    assert_true(NULL != blockchain->block_list);
    uint64_t num_blocks = 0;
    return_code = linked_list_length(blockchain->block_list, &num_blocks);
    assert_true(SUCCESS == return_code);
    assert_true(4 == num_blocks);
    block_t *block1 = (block_t *)blockchain->block_list->head->data;
    assert_true(GENESIS_BLOCK_PROOF_OF_WORK == block1->proof_of_work);
    sha_256_t empty_block_hash = {0};
    assert_true(0 == memcmp(
        &block1->previous_block_hash, &empty_block_hash, sizeof(sha_256_t)));
    bool transaction_list_is_empty = true;
    return_code = linked_list_is_empty(
        block1->transaction_list, &transaction_list_is_empty);
    assert_true(SUCCESS == return_code);
    assert_true(transaction_list_is_empty);
    block_t *block2 = (block_t *)blockchain->block_list->head->next->data;
    assert_true(0 != block2->proof_of_work);
    assert_true(0 != block2->created_at);
    assert_true(0 != memcmp(
        &block2->previous_block_hash, &empty_block_hash, sizeof(sha_256_t)));
    uint64_t num_transactions = 0;
    return_code = linked_list_length(
        block2->transaction_list, &num_transactions);
    assert_true(SUCCESS == return_code);
    assert_true(1 == num_transactions);
    transaction_t *transaction =
        (transaction_t *)block2->transaction_list->head->data;
    assert_true(AMOUNT_GENERATED_DURING_MINTING == transaction->amount);
    assert_true(0 != transaction->created_at);
    ssh_key_t empty_key = {0};
    assert_true(0 != memcmp(
        &transaction->sender_public_key, &empty_key, sizeof(ssh_key_t)));
    assert_true(0 == memcmp(
        &transaction->sender_public_key,
        &transaction->recipient_public_key,
        sizeof(ssh_key_t)));
    ssh_signature_t empty_signature = {0};
    assert_true(0 != memcmp(
        &transaction->sender_signature,
        &empty_signature,
        sizeof(ssh_signature_t)));
    block_t *block3 = (block_t *)blockchain->block_list->head->next->next->data;
    assert_true(0 != block3->proof_of_work);
    assert_true(0 != block3->created_at);
    assert_true(0 != memcmp(
        &block3->previous_block_hash, &empty_block_hash, sizeof(sha_256_t)));
    return_code = linked_list_length(
        block3->transaction_list, &num_transactions);
    assert_true(SUCCESS == return_code);
    assert_true(1 == num_transactions);
    transaction = (transaction_t *)block3->transaction_list->head->data;
    assert_true(AMOUNT_GENERATED_DURING_MINTING == transaction->amount);
    assert_true(0 != transaction->created_at);
    assert_true(0 != memcmp(
        &transaction->sender_public_key, &empty_key, sizeof(ssh_key_t)));
    assert_true(0 == memcmp(
        &transaction->sender_public_key,
        &transaction->recipient_public_key,
        sizeof(ssh_key_t)));
    assert_true(0 != memcmp(
        &transaction->sender_signature,
        &empty_signature,
        sizeof(ssh_signature_t)));
    blockchain_destroy(blockchain);
}

void test_blockchain_read_from_file_fails_on_invalid_input() {
    char fixture_directory[TESTS_MAX_PATH];
    get_fixture_directory(fixture_directory);
    char infile[TESTS_MAX_PATH];
    int return_value = snprintf(
        infile,
        TESTS_MAX_PATH,
        "%s/%s",
        fixture_directory,
        "blockchain_4_blocks_no_transactions");
    assert_true(return_value < TESTS_MAX_PATH);
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_read_from_file(NULL, infile);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = blockchain_read_from_file(&blockchain, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_blockchain_serialization_does_not_alter_block_hash() {
    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block = NULL;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    // Set created_at to get a consistent hash between test suite executions.
    // This is just for debugging. We might want to print out the hashes and
    // make sure they don't change between runs as a sanity check.
    genesis_block->created_at = 1;
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    sha_256_t genesis_block_hash_before_serialization = {0};
    return_code = block_hash(
        genesis_block, &genesis_block_hash_before_serialization);
    assert_true(SUCCESS == return_code);
    // TODO remove
    hash_print(&genesis_block_hash_before_serialization);
    printf("\n");
    printf("Serialized genesis block created_at: %lld\n", genesis_block->created_at);


    unsigned char *buffer = NULL;
    uint64_t buffer_size = 0;
    return_code = blockchain_serialize(blockchain, &buffer, &buffer_size);
    assert_true(SUCCESS == return_code);
    blockchain_t *deserialized_blockchain = NULL;
    return_code = blockchain_deserialize(
        &deserialized_blockchain, buffer, buffer_size);
    assert_true(SUCCESS == return_code);
    block_t *deserialized_genesis_block =
        (block_t *)deserialized_blockchain->block_list->head->data;
    sha_256_t genesis_block_hash_after_serialization = {0};
    return_code = block_hash(
        deserialized_genesis_block, &genesis_block_hash_after_serialization);
    assert_true(SUCCESS == return_code);
    // TODO remove
    hash_print(&genesis_block_hash_after_serialization);
    printf("\n");
    printf("Deserialized genesis block created_at: %lld\n", deserialized_genesis_block->created_at);

    assert_true(0 == memcmp(
        &genesis_block_hash_before_serialization,
        &genesis_block_hash_after_serialization,
        sizeof(sha_256_t)));
    free(buffer);
    blockchain_destroy(blockchain);
    blockchain_destroy(deserialized_blockchain);
}
