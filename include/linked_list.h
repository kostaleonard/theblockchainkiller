/**
 * @brief Defines a linked list.
 */

#ifndef INCLUDE_LINKED_LIST_H_
#define INCLUDE_LINKED_LIST_H_

#include <stdbool.h>
#include "include/return_codes.h"

//TODO docstring
typedef struct node_t {
    void *data;
    struct node_t *next;
} node_t;

//TODO docstring
typedef void (free_function_t(void *data));

//TODO docstring
typedef int (compare_function_t(void *data1, void *data2));

//TODO docstring
typedef struct linked_list_t {
    free_function_t *free_function;
    compare_function_t *compare_function;
    node_t *head;
} linked_list_t;

//TODO docstring
return_code_t linked_list_create(
    linked_list_t **linked_list,
    free_function_t free_function,
    compare_function_t compare_function
);

//TODO docstring
return_code_t linked_list_destroy(linked_list_t *linked_list);

//TODO docstring
return_code_t linked_list_prepend(linked_list_t *linked_list, void *data);

//TODO docstring
return_code_t linked_list_get_first(linked_list_t *linked_list, node_t **node);

//TODO docstring
return_code_t linked_list_remove_first(linked_list_t *linked_list);

//TODO docstring
return_code_t linked_list_is_empty(linked_list_t *linked_list, bool *is_empty);

#endif  // INCLUDE_LINKED_LIST_H_
