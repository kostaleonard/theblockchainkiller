/**
 * @brief Tests example.c.
 */

#ifndef TEST_EXAMPLE_H
#define TEST_EXAMPLE_H
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_square_returns_perfect_square();

void test_square_returns_positive_value_on_negative_input();

#endif
