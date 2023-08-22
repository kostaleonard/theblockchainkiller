/**
 * @brief Tests linked_list.c
 */

#ifndef TESTS_TEST_LINKED_LIST_H_
#define TESTS_TEST_LINKED_LIST_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_linked_list_create_returns_linked_list();

void test_linked_list_create_fails_on_invalid_input();

void test_linked_list_destroy_returns_success();

#endif  // TESTS_TEST_LINKED_LIST_H_
