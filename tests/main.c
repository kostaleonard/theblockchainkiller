/**
 * @brief Runs the unit test suite.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "tests/test_linked_list.h"

int main(int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_linked_list_create_returns_linked_list),
        cmocka_unit_test(test_linked_list_create_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_destroy_empty_list_returns_success),
        cmocka_unit_test(
            test_linked_list_destroy_nonempty_list_returns_success),
        cmocka_unit_test(test_linked_list_destroy_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_prepend_adds_node_to_front),
        cmocka_unit_test(test_linked_list_prepend_fails_on_invalid_input),
        cmocka_unit_test(
            test_linked_list_get_first_gives_null_and_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_get_first_gives_head_of_list),
        cmocka_unit_test(test_linked_list_get_first_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_remove_first_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_remove_first_removes_head),
        cmocka_unit_test(test_linked_list_remove_first_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_is_empty_gives_true_if_no_elements),
        cmocka_unit_test(
            test_linked_list_is_empty_gives_false_if_list_has_elements),
        cmocka_unit_test(test_linked_list_is_empty_fails_on_invalid_input),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
