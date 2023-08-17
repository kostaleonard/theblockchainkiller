/**
 * @brief Tests example.c.
 */

#ifndef TESTS_TEST_EXAMPLE_H_
#define TESTS_TEST_EXAMPLE_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_square_returns_perfect_square();

void test_square_returns_positive_value_on_negative_input();

#endif  // TESTS_TEST_EXAMPLE_H_
