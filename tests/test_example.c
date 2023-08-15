#include "example.h"
#include "test_example.h"
//TODO remove leak
#include <stdio.h>
#include <stdlib.h>

void test_square_returns_perfect_square()
{
    //TODO remove leak
    void *p = malloc(50);
    printf("%p\n", p);
    assert_true(9 == square(3));
    assert_true(25 == square(5));
}

void test_square_returns_positive_value_on_negative_input()
{
    assert_true(1 == square(-1));
    assert_true(4 == square(-2));
}
