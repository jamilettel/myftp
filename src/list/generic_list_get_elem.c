/*
** EPITECH PROJECT, 2020
** get elem
** File description:
** generic list
*/

#include "generic_list.h"
#include <stddef.h>

void *list_get_elem_at_front(list_t list)
{
    if (list_is_empty(list))
        return (NULL);
    return (list->value);
}

void *list_get_elem_at_back(list_t list)
{
    if (list_is_empty(list))
        return (NULL);
    while (list->next)
        list = list->next;
    return (list->value);
}

void *list_get_elem_at_position(list_t list, unsigned int pos)
{
    if (list_is_empty(list))
        return (NULL);
    for (unsigned int i = 0; i < pos; i++) {
        if (!list->next)
            break;
        list = list->next;
    }
    return (list ? list->value : 0);
}

list_t list_get_first_node_with_value(list_t list, void *value,
value_comparator_t val_comp)
{
    if (list_is_empty(list))
        return (NULL);
    while (list) {
        if (!val_comp(list->value, value))
            return (list);
        list = list->next;
    }
    return (NULL);
}
