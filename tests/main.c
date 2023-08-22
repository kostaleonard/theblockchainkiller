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
        cmocka_unit_test(test_linked_list_destroy_returns_success),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
