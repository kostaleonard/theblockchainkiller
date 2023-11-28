#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "include/transaction.h"
#include "include/return_codes.h"
#include "tests/test_transaction.h"

// These environment variables need to contain a valid RSA key pair.
#define TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE "THEBLOCKCHAINKILLER_TEST_PRIVATE_KEY"
#define TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE "THEBLOCKCHAINKILLER_TEST_PUBLIC_KEY"

void test_transaction_create_gives_transaction() {
    transaction_t *transaction = NULL;
    ssh_key_t user_1_key = {0};
    user_1_key.bytes[0] = '1';
    ssh_key_t user_2_key = {0};
    user_2_key.bytes[0] = '2';
    uint32_t amount = 5;
    ssh_key_t user_1_private_key = {0};
    user_1_private_key.bytes[0] = '3';
    return_code_t return_code = transaction_create(
        &transaction,
        &user_1_key,
        &user_2_key,
        amount,
        &user_1_private_key);
    assert_true(SUCCESS == return_code);
    assert_true(0 == memcmp(
        &transaction->sender_public_key, &user_1_key, MAX_SSH_KEY_LENGTH));
    assert_true(0 == memcmp(
        &transaction->recipient_public_key, &user_2_key, MAX_SSH_KEY_LENGTH));
    assert_true(transaction->amount == amount);
    assert_true(0 != transaction->created_at);
    char empty_signature[MAX_SSH_SIGNATURE_LENGTH] = {0};
    assert_true(0 != memcmp(
        &transaction->sender_signature,
        empty_signature,
        MAX_SSH_SIGNATURE_LENGTH));
    transaction_destroy(transaction);
}

void test_transaction_create_fails_on_invalid_input() {
    transaction_t *transaction = NULL;
    ssh_key_t user_1_key = {0};
    user_1_key.bytes[0] = '1';
    ssh_key_t user_2_key = {0};
    user_2_key.bytes[0] = '2';
    uint32_t amount = 5;
    ssh_key_t user_1_private_key = {0};
    user_1_private_key.bytes[0] = '3';
    return_code_t return_code = transaction_create(
        NULL,
        &user_1_key,
        &user_2_key,
        amount,
        &user_1_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        NULL,
        &user_2_key,
        amount,
        &user_1_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        &user_1_key,
        NULL,
        amount,
        &user_1_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        &user_1_key,
        &user_2_key,
        amount,
        NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_transaction_destroy_returns_success() {
    transaction_t *transaction = NULL;
    ssh_key_t user_1_key = {0};
    user_1_key.bytes[0] = '1';
    ssh_key_t user_2_key = {0};
    user_2_key.bytes[0] = '2';
    uint32_t amount = 5;
    ssh_key_t user_1_private_key = {0};
    user_1_private_key.bytes[0] = '3';
    return_code_t return_code = transaction_create(
        &transaction,
        &user_1_key,
        &user_2_key,
        amount,
        &user_1_private_key);
    assert_true(SUCCESS == return_code);
    return_code = transaction_destroy(transaction);
    assert_true(SUCCESS == return_code);
}

void test_transaction_destroy_fails_on_invalid_input() {
    return_code_t return_code = transaction_destroy(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_transaction_generate_signature_gives_signature() {
    transaction_t transaction = {0};
    // TODO not sure if getenv will work because it doesn't allow multi-line variables.
    char *ssh_public_key_contents = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    assert_true(NULL != ssh_public_key_contents);
    char *ssh_private_key_contents = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    assert_true(NULL != ssh_private_key_contents);
    ssh_key_t ssh_public_key = {0};
    memcpy(
        &ssh_public_key.bytes,
        ssh_public_key_contents,
        sizeof(ssh_public_key.bytes));
    ssh_key_t ssh_private_key = {0};
    memcpy(
        &ssh_private_key.bytes,
        ssh_private_key_contents,
        sizeof(ssh_private_key.bytes));
    transaction.created_at = time(NULL);
    memcpy(
        &transaction.sender_public_key,
        &ssh_public_key,
        sizeof(transaction.sender_public_key));
    memcpy(
        &transaction.recipient_public_key,
        &ssh_public_key,
        sizeof(transaction.recipient_public_key));
    transaction.amount = 17;
    ssh_signature_t signature = {0};
    return_code_t return_code = transaction_generate_signature(
        &signature, &transaction, &ssh_private_key);
    assert_true(SUCCESS == return_code);
    char empty_signature[MAX_SSH_SIGNATURE_LENGTH] = {0};
    assert_true(0 == memcmp(
        &transaction.sender_signature,
        empty_signature,
        MAX_SSH_SIGNATURE_LENGTH));
    assert_true(0 != memcmp(
        &signature,
        empty_signature,
        MAX_SSH_SIGNATURE_LENGTH));
}

void test_transaction_generate_signature_fails_on_invalid_input() {
    transaction_t transaction = {0};
    char *ssh_public_key_contents = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    assert_true(NULL != ssh_public_key_contents);
    char *ssh_private_key_contents = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    assert_true(NULL != ssh_private_key_contents);
    ssh_key_t ssh_public_key = {0};
    memcpy(
        &ssh_public_key.bytes,
        ssh_public_key_contents,
        sizeof(ssh_public_key.bytes));
    ssh_key_t ssh_private_key = {0};
    memcpy(
        &ssh_private_key.bytes,
        ssh_private_key_contents,
        sizeof(ssh_private_key.bytes));
    transaction.created_at = time(NULL);
    memcpy(
        &transaction.sender_public_key,
        &ssh_public_key,
        sizeof(transaction.sender_public_key));
    memcpy(
        &transaction.recipient_public_key,
        &ssh_public_key,
        sizeof(transaction.recipient_public_key));
    transaction.amount = 17;
    ssh_signature_t signature = {0};
    return_code_t return_code = transaction_generate_signature(
        NULL, &transaction, &ssh_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_generate_signature(
        &signature, NULL, &ssh_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_generate_signature(
        &signature, &transaction, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_transaction_verify_signature_identifies_valid_signature() {
    // TODO
}

void test_transaction_verify_signature_identifies_invalid_signature() {
    // TODO
}

void test_transaction_verify_signature_fails_on_invalid_input() {
    // TODO
}
