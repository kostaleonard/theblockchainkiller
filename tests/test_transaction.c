#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "include/base64.h"
#include "include/transaction.h"
#include "include/return_codes.h"
#include "tests/test_transaction.h"

// These environment variables need to contain a base64 encoded RSA key pair.
#define TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE \
    "THEBLOCKCHAINKILLER_TEST_PRIVATE_KEY"
#define TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE \
    "THEBLOCKCHAINKILLER_TEST_PUBLIC_KEY"

void test_transaction_create_gives_transaction() {
    transaction_t *transaction = NULL;
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t sender_public_key = {0};
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t ssh_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        ssh_private_key.bytes);
    // In these test transactions, the sender and recipient are the same.
    ssh_key_t recipient_public_key = {0};
    memcpy(
        &recipient_public_key,
        &sender_public_key,
        sizeof(recipient_public_key));
    uint32_t amount = 5;
    return_code = transaction_create(
        &transaction,
        &sender_public_key,
        &recipient_public_key,
        amount,
        &ssh_private_key);
    assert_true(SUCCESS == return_code);
    assert_true(0 == memcmp(
        &transaction->sender_public_key,
        &sender_public_key,
        MAX_SSH_KEY_LENGTH));
    assert_true(0 == memcmp(
        &transaction->recipient_public_key,
        &recipient_public_key,
        MAX_SSH_KEY_LENGTH));
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
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t sender_public_key = {0};
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t ssh_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        ssh_private_key.bytes);
    // In these test transactions, the sender and recipient are the same.
    ssh_key_t recipient_public_key = {0};
    memcpy(
        &recipient_public_key,
        &sender_public_key,
        sizeof(recipient_public_key));
    uint32_t amount = 5;
    return_code = transaction_create(
        NULL,
        &sender_public_key,
        &recipient_public_key,
        amount,
        &ssh_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        NULL,
        &recipient_public_key,
        amount,
        &ssh_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        &sender_public_key,
        NULL,
        amount,
        &ssh_private_key);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        &sender_public_key,
        &recipient_public_key,
        amount,
        NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_transaction_destroy_returns_success() {
    transaction_t *transaction = NULL;
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t sender_public_key = {0};
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t ssh_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        ssh_private_key.bytes);
    // In these test transactions, the sender and recipient are the same.
    ssh_key_t recipient_public_key = {0};
    memcpy(
        &recipient_public_key,
        &sender_public_key,
        sizeof(recipient_public_key));
    uint32_t amount = 5;
    return_code = transaction_create(
        &transaction,
        &sender_public_key,
        &recipient_public_key,
        amount,
        &ssh_private_key);
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
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        transaction.sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t ssh_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        ssh_private_key.bytes);
    transaction.created_at = time(NULL);
    // In these test transactions, the sender and recipient are the same.
    memcpy(
        &transaction.recipient_public_key,
        &transaction.sender_public_key,
        sizeof(transaction.recipient_public_key));
    transaction.amount = 17;
    ssh_signature_t signature = {0};
    return_code = transaction_generate_signature(
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
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        transaction.sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t ssh_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        ssh_private_key.bytes);
    transaction.created_at = time(NULL);
    // In these test transactions, the sender and recipient are the same.
    memcpy(
        &transaction.recipient_public_key,
        &transaction.sender_public_key,
        sizeof(transaction.recipient_public_key));
    transaction.amount = 17;
    ssh_signature_t signature = {0};
    return_code = transaction_generate_signature(
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
    transaction_t transaction = {0};
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        transaction.sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t ssh_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        ssh_private_key.bytes);
    transaction.created_at = time(NULL);
    // In these test transactions, the sender and recipient are the same.
    memcpy(
        &transaction.recipient_public_key,
        &transaction.sender_public_key,
        sizeof(transaction.recipient_public_key));
    transaction.amount = 17;
    return_code = transaction_generate_signature(
        &transaction.sender_signature, &transaction, &ssh_private_key);
    assert_true(SUCCESS == return_code);
    bool is_valid_signature = false;
    return_code = transaction_verify_signature(
        &is_valid_signature, &transaction);
    assert_true(SUCCESS == return_code);
    assert_true(is_valid_signature);
}

void test_transaction_verify_signature_identifies_invalid_signature() {
    transaction_t transaction = {0};
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        transaction.sender_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    ssh_key_t ssh_private_key = {0};
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        ssh_private_key.bytes);
    transaction.created_at = time(NULL);
    // In these test transactions, the sender and recipient are the same.
    memcpy(
        &transaction.recipient_public_key,
        &transaction.sender_public_key,
        sizeof(transaction.recipient_public_key));
    transaction.amount = 17;
    // Fill the signature with garbage bytes.
    for (size_t idx = 0;
        idx < sizeof(transaction.sender_signature.bytes);
        idx++) {
        transaction.sender_signature.bytes[idx] = idx & 0xff;
    }
    bool is_valid_signature = false;
    return_code = transaction_verify_signature(
        &is_valid_signature, &transaction);
    assert_true(SUCCESS == return_code);
    assert_true(!is_valid_signature);
}

void test_transaction_verify_signature_fails_on_invalid_input() {
    transaction_t transaction = {0};
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    char ssh_public_key_contents[MAX_SSH_KEY_LENGTH] = {0};
    return_code_t return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        ssh_public_key_contents);
    assert_true(NULL != ssh_public_key_contents);
    ssh_key_t ssh_public_key = {0};
    memcpy(
        &ssh_public_key.bytes,
        ssh_public_key_contents,
        sizeof(ssh_public_key.bytes));
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
    // Fill the signature with garbage bytes.
    for (size_t idx = 0; idx < sizeof(signature.bytes); idx++) {
        signature.bytes[idx] = idx & 0xff;
    }
    bool is_valid_signature = false;
    return_code = transaction_verify_signature(
        NULL, &transaction);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_verify_signature(
        &is_valid_signature, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}
