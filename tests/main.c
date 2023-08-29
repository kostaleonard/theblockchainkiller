/**
 * @brief Runs the unit test suite.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "tests/test_linked_list.h"
#include "tests/test_block.h"
#include "tests/test_blockchain.h"

int main(int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        // test_linked_list.h
        cmocka_unit_test(test_linked_list_create_gives_linked_list),
        cmocka_unit_test(test_linked_list_create_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_destroy_empty_list_returns_success),
        cmocka_unit_test(
            test_linked_list_destroy_nonempty_list_returns_success),
        cmocka_unit_test(test_linked_list_destroy_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_prepend_adds_node_to_front),
        cmocka_unit_test(test_linked_list_prepend_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_get_first_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_get_first_gives_head_of_list),
        cmocka_unit_test(test_linked_list_get_first_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_remove_first_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_remove_first_removes_head),
        cmocka_unit_test(test_linked_list_remove_first_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_is_empty_gives_true_if_no_elements),
        cmocka_unit_test(
            test_linked_list_is_empty_gives_false_if_list_has_elements),
        cmocka_unit_test(test_linked_list_is_empty_fails_on_invalid_input),
        cmocka_unit_test(
            test_linked_list_find_succeeds_and_gives_null_if_list_empty),
        cmocka_unit_test(
            test_linked_list_find_succeeds_and_gives_null_if_no_match),
        cmocka_unit_test(test_linked_list_find_gives_first_matching_element),
        cmocka_unit_test(test_linked_list_find_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_append_adds_node_to_back),
        cmocka_unit_test(test_linked_list_append_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_get_last_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_get_last_gives_last_element),
        cmocka_unit_test(test_linked_list_get_last_fails_on_invalid_input),
        // test_block.h
        cmocka_unit_test(test_block_create_gives_block),
        cmocka_unit_test(test_block_create_fails_on_invalid_input),
        cmocka_unit_test(
            test_create_genesis_block_gives_block_with_genesis_values),
        cmocka_unit_test(test_create_genesis_block_fails_on_invalid_input),
        cmocka_unit_test(test_block_destroy_returns_success),
        cmocka_unit_test(test_block_destroy_fails_on_invalid_input),
        // test_blockchain.h
        cmocka_unit_test(test_blockchain_create_gives_blockchain),
        cmocka_unit_test(test_blockchain_create_fails_on_invalid_input),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
