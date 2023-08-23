#include <stdlib.h>
#include "include/linked_list.h"

return_code_t linked_list_create(
    linked_list_t **linked_list,
    free_function_t free_function,
    compare_function_t compare_function
) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == free_function || NULL == compare_function) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    linked_list_t *list = malloc(sizeof(linked_list_t));
    if (NULL == list) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    list->free_function = free_function;
    list->compare_function = compare_function;
    list->head = NULL;
    *linked_list = list;
end:
    return return_code;
}

return_code_t linked_list_destroy(linked_list_t *linked_list) {
    //TODO
    return FAILURE_INVALID_INPUT;
}

return_code_t linked_list_prepend(linked_list_t *linked_list, void *data) {
    //TODO
    return FAILURE_INVALID_INPUT;
}

return_code_t linked_list_get_first(linked_list_t *linked_list, node_t **node) {
    //TODO
    return FAILURE_INVALID_INPUT;
}

return_code_t linked_list_remove_first(linked_list_t *linked_list) {
    //TODO
    return FAILURE_INVALID_INPUT;
}

return_code_t linked_list_is_empty(linked_list_t *linked_list, bool *is_empty) {
    //TODO
    return FAILURE_INVALID_INPUT;
}
