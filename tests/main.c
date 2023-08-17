/**
 * @brief Runs the unit test suite.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "tests/test_example.h"

int main(int argc, char **argv)
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_square_returns_perfect_square),
        cmocka_unit_test(test_square_returns_positive_value_on_negative_input),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
