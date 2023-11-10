#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "include/transaction.h"
#include "include/return_codes.h"
#include "tests/test_transaction.h"

void test_transaction_create_gives_transaction() {
    transaction_t *transaction = NULL;
    ssh_key_t user_1_key = {0};
    user_1_key.bytes[0] = '1';
    ssh_key_t user_2_key = {0};
    user_2_key.bytes[0] = '2';
    uint32_t amount = 5;
    return_code_t return_code = transaction_create(
        &transaction,
        &user_1_key,
        &user_2_key,
        amount);
    assert_true(SUCCESS == return_code);
    assert_true(0 == memcmp(
        &transaction->sender_public_key, &user_1_key, MAX_SSH_KEY_LENGTH));
    assert_true(0 == memcmp(
        &transaction->recipient_public_key, &user_2_key, MAX_SSH_KEY_LENGTH));
    assert_true(transaction->amount == amount);
    assert_true(0 != transaction->created_at);
    transaction_destroy(transaction);
}

void test_transaction_create_fails_on_invalid_input() {
    transaction_t *transaction = NULL;
    ssh_key_t user_1_key = {0};
    user_1_key.bytes[0] = '1';
    ssh_key_t user_2_key = {0};
    user_2_key.bytes[0] = '2';
    uint32_t amount = 5;
    return_code_t return_code = transaction_create(
        NULL,
        &user_1_key,
        &user_2_key,
        amount);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        NULL,
        &user_2_key,
        amount);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = transaction_create(
        &transaction,
        &user_1_key,
        NULL,
        amount);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_transaction_destroy_returns_success() {
    transaction_t *transaction = NULL;
    ssh_key_t user_1_key = {0};
    user_1_key.bytes[0] = '1';
    ssh_key_t user_2_key = {0};
    user_2_key.bytes[0] = '2';
    uint32_t amount = 5;
    return_code_t return_code = transaction_create(
        &transaction,
        &user_1_key,
        &user_2_key,
        amount);
    assert_true(SUCCESS == return_code);
    return_code = transaction_destroy(transaction);
    assert_true(SUCCESS == return_code);
}

void test_transaction_destroy_fails_on_invalid_input() {
    return_code_t return_code = transaction_destroy(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}
