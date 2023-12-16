/**
 * @brief Tests transaction.c
 */

#ifndef TESTS_TEST_TRANSACTION_H_
#define TESTS_TEST_TRANSACTION_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_transaction_create_gives_transaction();

void test_transaction_create_fails_on_invalid_input();

void test_transaction_destroy_returns_success();

void test_transaction_destroy_fails_on_invalid_input();

void test_transaction_generate_signature_gives_signature();

void test_transaction_generate_signature_fails_on_invalid_input();

void test_transaction_verify_signature_identifies_valid_signature();

void test_transaction_verify_signature_identifies_invalid_signature();

void test_transaction_verify_signature_fails_on_invalid_input();

#endif  // TESTS_TEST_TRANSACTION_H_
