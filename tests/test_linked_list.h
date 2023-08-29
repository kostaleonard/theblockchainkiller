/**
 * @brief Tests linked_list.c
 */

#ifndef TESTS_TEST_LINKED_LIST_H_
#define TESTS_TEST_LINKED_LIST_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_linked_list_create_gives_linked_list();

void test_linked_list_create_fails_on_invalid_input();

void test_linked_list_create_compare_function_may_be_null();

void test_linked_list_destroy_empty_list_returns_success();

void test_linked_list_destroy_nonempty_list_returns_success();

void test_linked_list_destroy_fails_on_invalid_input();

void test_linked_list_prepend_adds_node_to_front();

void test_linked_list_prepend_fails_on_invalid_input();

void test_linked_list_get_first_fails_on_empty_list();

void test_linked_list_get_first_gives_head_of_list();

void test_linked_list_get_first_fails_on_invalid_input();

void test_linked_list_remove_first_fails_on_empty_list();

void test_linked_list_remove_first_removes_head();

void test_linked_list_remove_first_fails_on_invalid_input();

void test_linked_list_is_empty_gives_true_if_no_elements();

void test_linked_list_is_empty_gives_false_if_list_has_elements();

void test_linked_list_is_empty_fails_on_invalid_input();

void test_linked_list_find_succeeds_and_gives_null_if_list_empty();

void test_linked_list_find_succeeds_and_gives_null_if_no_match();

void test_linked_list_find_gives_first_matching_element();

void test_linked_list_find_fails_on_invalid_input();

void test_linked_list_find_fails_on_null_compare_function();

void test_linked_list_append_adds_node_to_back();

void test_linked_list_append_fails_on_invalid_input();

void test_linked_list_get_last_fails_on_empty_list();

void test_linked_list_get_last_gives_last_element();

void test_linked_list_get_last_fails_on_invalid_input();

#endif  // TESTS_TEST_LINKED_LIST_H_
