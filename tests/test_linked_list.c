#include <stdlib.h>
#include <stdbool.h>
#include "include/linked_list.h"
#include "include/return_codes.h"
#include "tests/test_linked_list.h"

int compare_ints(int *num1, int *num2) {
    if (NULL == num1 || NULL == num2) {
        return 0;
    }
    return *num1 - *num2;
}

void test_linked_list_create_gives_linked_list() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    assert_true(NULL != list);
    assert_true(SUCCESS == return_code);
    return_code = linked_list_destroy(list);
}

void test_linked_list_create_fails_on_invalid_input() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        NULL,
        free,
        (compare_function_t *)compare_ints);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_create(
        &list,
        NULL,
        (compare_function_t *)compare_ints);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_linked_list_create_compare_function_may_be_null() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        NULL);
    assert_true(NULL != list);
    assert_true(SUCCESS == return_code);
    return_code = linked_list_destroy(list);
}

void test_linked_list_destroy_empty_list_returns_success() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    return_code = linked_list_destroy(list);
    assert_true(SUCCESS == return_code);
}

void test_linked_list_destroy_nonempty_list_returns_success() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
    }
end:
    return_code = linked_list_destroy(list);
    assert_true(SUCCESS == return_code);
}

void test_linked_list_destroy_fails_on_invalid_input() {
    return_code_t return_code = linked_list_destroy(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_linked_list_prepend_adds_node_to_front() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
        node_t *node = NULL;
        return_code = linked_list_get_first(list, &node);
        assert_true(SUCCESS == return_code);
        assert_true(NULL != node);
        assert_true(idx == *((int *)node->data));
    }
end:
    return_code = linked_list_destroy(list);
    assert_true(SUCCESS == return_code);
}

void test_linked_list_prepend_fails_on_invalid_input() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    int *data = malloc(sizeof(int));
    if (NULL == data) {
        assert_true(false);
        goto end;
    }
    *data = 17;
    return_code = linked_list_prepend(NULL, data);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_prepend(list, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    free(data);
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_get_first_fails_on_empty_list() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    node_t *node = NULL;
    return_code = linked_list_get_first(list, &node);
    assert_true(FAILURE_LINKED_LIST_EMPTY == return_code);
    assert_true(NULL == node);
    return_code = linked_list_destroy(list);
}

void test_linked_list_get_first_gives_head_of_list() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    int *data = malloc(sizeof(int));
    if (NULL == data) {
        assert_true(false);
        goto end;
    }
    *data = 17;
    return_code = linked_list_prepend(list, data);
    node_t *node = NULL;
    return_code = linked_list_get_first(list, &node);
    assert_true(SUCCESS == return_code);
    assert_true(node == list->head);
    assert_true(17 == *((int *)node->data));
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_get_first_fails_on_invalid_input() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    node_t *node = NULL;
    return_code = linked_list_get_first(NULL, &node);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_get_first(list, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_destroy(list);
}

void test_linked_list_remove_first_fails_on_empty_list() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    return_code = linked_list_remove_first(list);
    assert_true(FAILURE_LINKED_LIST_EMPTY == return_code);
    return_code = linked_list_destroy(list);
}

void test_linked_list_remove_first_removes_head() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
    }
    return_code = linked_list_remove_first(list);
    assert_true(SUCCESS == return_code);
    node_t *node = NULL;
    return_code = linked_list_get_first(list, &node);
    assert_true(NULL != node);
    assert_true(8 == *((int *)node->data));
end:
    return_code = linked_list_destroy(list);
    assert_true(SUCCESS == return_code);
}

void test_linked_list_remove_first_fails_on_invalid_input() {
    return_code_t return_code = linked_list_remove_first(NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
}

void test_linked_list_is_empty_gives_true_if_no_elements() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    bool is_empty = false;
    return_code = linked_list_is_empty(list, &is_empty);
    assert_true(SUCCESS == return_code);
    assert_true(is_empty);
    return_code = linked_list_destroy(list);
}

void test_linked_list_is_empty_gives_false_if_list_has_elements() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
    }
    bool is_empty = false;
    return_code = linked_list_is_empty(list, &is_empty);
    assert_true(SUCCESS == return_code);
    assert_true(!is_empty);
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_is_empty_fails_on_invalid_input() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    bool is_empty = false;
    return_code = linked_list_is_empty(NULL, &is_empty);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_is_empty(list, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_destroy(list);
}

void test_linked_list_find_succeeds_and_gives_null_if_list_empty() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    node_t *node = NULL;
    int data = 5;
    return_code = linked_list_find(list, &data, &node);
    assert_true(SUCCESS == return_code);
    assert_true(NULL == node);
    return_code = linked_list_destroy(list);
}

void test_linked_list_find_succeeds_and_gives_null_if_no_match() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
    }
    node_t *node = NULL;
    int data = 20;
    return_code = linked_list_find(list, &data, &node);
    assert_true(SUCCESS == return_code);
    assert_true(NULL == node);
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_find_gives_first_matching_element() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
    }
    node_t *node = NULL;
    int data = 7;
    return_code = linked_list_find(list, &data, &node);
    assert_true(SUCCESS == return_code);
    assert_true(list->head->next->next == node);
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_find_fails_on_invalid_input() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    node_t *node = NULL;
    int data = 5;
    return_code = linked_list_find(NULL, &data, &node);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_find(list, NULL, &node);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_find(list, &data, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_destroy(list);
}

void test_linked_list_find_fails_on_null_compare_function() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        NULL);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
    }
    node_t *node = NULL;
    int data = 7;
    return_code = linked_list_find(list, &data, &node);
    assert_true(FAILURE_LINKED_LIST_NO_COMPARE_FUNCTION == return_code);
    assert_true(NULL == node);
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_append_adds_node_to_back() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_append(list, data);
        node_t *node = NULL;
        return_code = linked_list_get_last(list, &node);
        assert_true(SUCCESS == return_code);
        assert_true(NULL != node);
        assert_true(idx == *((int *)node->data));
    }
end:
    return_code = linked_list_destroy(list);
    assert_true(SUCCESS == return_code);
}

void test_linked_list_append_fails_on_invalid_input() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    int *data = malloc(sizeof(int));
    if (NULL == data) {
        assert_true(false);
        goto end;
    }
    *data = 17;
    return_code = linked_list_append(NULL, data);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_append(list, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    free(data);
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_get_last_fails_on_empty_list() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    node_t *node = NULL;
    return_code = linked_list_get_last(list, &node);
    assert_true(FAILURE_LINKED_LIST_EMPTY == return_code);
    assert_true(NULL == node);
    return_code = linked_list_destroy(list);
}

void test_linked_list_get_last_gives_last_element() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    for (int idx = 0; idx < 10; idx++) {
        int *data = malloc(sizeof(int));
        if (NULL == data) {
            assert_true(false);
            goto end;
        }
        *data = idx;
        return_code = linked_list_prepend(list, data);
        node_t *node = NULL;
        return_code = linked_list_get_last(list, &node);
        assert_true(SUCCESS == return_code);
        assert_true(NULL != node);
        assert_true(0 == *((int *)node->data));
    }
end:
    return_code = linked_list_destroy(list);
}

void test_linked_list_get_last_fails_on_invalid_input() {
    linked_list_t *list = NULL;
    return_code_t return_code = linked_list_create(
        &list,
        free,
        (compare_function_t *)compare_ints);
    node_t *node = NULL;
    return_code = linked_list_get_last(NULL, &node);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_get_last(list, NULL);
    assert_true(FAILURE_INVALID_INPUT == return_code);
    return_code = linked_list_destroy(list);
}
