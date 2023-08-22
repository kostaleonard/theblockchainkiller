#include <stdlib.h>
#include "include/linked_list.h"
#include "include/return_codes.h"
#include "tests/test_linked_list.h"

int compare_ints(int *num1, int *num2) {
    if (NULL == num1 || NULL == num2) {
        return 0;
    }
    return *num2 - *num1;
}

void test_linked_list_create_returns_linked_list() {
    linked_list_t *list = linked_list_create(free, (compare_function_t *)compare_ints);
    assert_true(NULL != list);
    linked_list_destroy(list);
}

void test_linked_list_destroy_returns_success() {
    linked_list_t *list = linked_list_create(free, (compare_function_t *)compare_ints);
    return_code_t result = linked_list_destroy(list);
    assert_true(SUCCESS == result);
}
