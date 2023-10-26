#include <stdbool.h>
#include <stdint.h>
#include "include/transaction.h"
#include "include/return_codes.h"
#include "tests/test_transaction.h"

void test_transaction_create_gives_transaction() {
    transaction_t *transaction = NULL;
    uint32_t user_id1 = 1;
    uint32_t user_id2 = 2;
    uint32_t amount = 5;
    return_code_t return_code = transaction_create(
        &transaction,
        user_id1,
        user_id2,
        amount);
    assert_true(SUCCESS == return_code);
    assert_true(transaction->sender_id == user_id1);
    assert_true(transaction->recipient_id == user_id2);
    assert_true(transaction->amount == amount);
    assert_true(0 != transaction->created_at);
    transaction_destroy(transaction);
}

void test_transaction_create_fails_on_invalid_input() {
    uint32_t user_id1 = 1;
    uint32_t user_id2 = 2;
    uint32_t amount = 5;
    return_code_t return_code = transaction_create(
        NULL,
        user_id1,
        user_id2,
        amount);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_transaction_destroy_returns_success() {
    transaction_t *transaction = NULL;
    uint32_t user_id1 = 1;
    uint32_t user_id2 = 2;
    uint32_t amount = 5;
    return_code_t return_code = transaction_create(
        &transaction,
        user_id1,
        user_id2,
        amount);
    assert_true(SUCCESS == return_code);
    return_code = transaction_destroy(transaction);
    assert_true(SUCCESS == return_code);
}

void test_transaction_destroy_fails_on_invalid_input() {
    return_code_t return_code = transaction_destroy(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}
