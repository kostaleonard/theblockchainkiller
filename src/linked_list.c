#include <stdlib.h>
#include "include/linked_list.h"

return_code_t linked_list_create(
    linked_list_t **linked_list,
    free_function_t free_function,
    compare_function_t compare_function
) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == free_function) {
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
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    bool is_empty = false;
    return_code = linked_list_is_empty(linked_list, &is_empty);
    if (SUCCESS != return_code) {
        goto end;
    }
    while (!is_empty) {
        return_code = linked_list_remove_first(linked_list);
        if (SUCCESS != return_code) {
            goto end;
        }
        return_code = linked_list_is_empty(linked_list, &is_empty);
        if (SUCCESS != return_code) {
            goto end;
        }
    }
    free(linked_list);
end:
    return return_code;
}

return_code_t linked_list_prepend(linked_list_t *linked_list, void *data) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == data) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    node_t *node = malloc(sizeof(node_t));
    if (NULL == node) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    node->data = data;
    node->next = linked_list->head;
    linked_list->head = node;
end:
    return return_code;
}

return_code_t linked_list_get_first(linked_list_t *linked_list, node_t **node) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == node) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    bool is_empty = false;
    return_code = linked_list_is_empty(linked_list, &is_empty);
    if (SUCCESS != return_code) {
        goto end;
    }
    if (is_empty) {
        return_code = FAILURE_LINKED_LIST_EMPTY;
        goto end;
    }
    *node = linked_list->head;
end:
    return return_code;
}

return_code_t linked_list_remove_first(linked_list_t *linked_list) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    node_t *first = NULL;
    return_code = linked_list_get_first(linked_list, &first);
    if (SUCCESS != return_code) {
        goto end;
    }
    linked_list->head = first->next;
    linked_list->free_function(first->data);
    free(first);
end:
    return return_code;
}

return_code_t linked_list_is_empty(linked_list_t *linked_list, bool *is_empty) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == is_empty) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    *is_empty = NULL == linked_list->head;
end:
    return return_code;
}

return_code_t linked_list_find(
    linked_list_t *linked_list,
    void *data,
    node_t **node
) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == data || NULL == node) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    if (NULL == linked_list->compare_function) {
        return_code = FAILURE_LINKED_LIST_NO_COMPARE_FUNCTION;
        goto end;
    }
    node_t *ptr = linked_list->head;
    while (NULL != ptr) {
        if (0 == linked_list->compare_function(data, ptr->data)) {
            *node = ptr;
            goto end;
        }
        ptr = ptr->next;
    }
end:
    return return_code;
}

return_code_t linked_list_append(linked_list_t *linked_list, void *data) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == data) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    bool is_empty = false;
    return_code = linked_list_is_empty(linked_list, &is_empty);
    if (SUCCESS != return_code) {
        goto end;
    }
    if (is_empty) {
        return_code = linked_list_prepend(linked_list, data);
        goto end;
    }
    node_t *last = NULL;
    return_code = linked_list_get_last(linked_list, &last);
    if (SUCCESS != return_code) {
        goto end;
    }
    node_t *node = malloc(sizeof(node_t));
    if (NULL == node) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    node->data = data;
    node->next = NULL;
    last->next = node;
end:
    return return_code;
}

return_code_t linked_list_get_last(linked_list_t *linked_list, node_t **node) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == node) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    bool is_empty = false;
    return_code = linked_list_is_empty(linked_list, &is_empty);
    if (SUCCESS != return_code) {
        goto end;
    }
    if (is_empty) {
        return_code = FAILURE_LINKED_LIST_EMPTY;
        goto end;
    }
    node_t *ptr = linked_list->head;
    while (NULL != ptr->next) {
        ptr = ptr->next;
    }
    *node = ptr;
end:
    return return_code;
}

return_code_t linked_list_length(linked_list_t *linked_list, uint64_t *length) {
    return_code_t return_code = SUCCESS;
    if (NULL == linked_list || NULL == length) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    uint64_t result = 0;
    for (node_t *node = linked_list->head; NULL != node; node = node->next) {
        result++;
    }
    *length = result;
end:
    return return_code;
}
