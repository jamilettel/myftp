/*
** EPITECH PROJECT, 2020
** generic list
** File description:
** ex04
*/

#include "generic_list.h"

bool list_del_node(list_t *front_ptr, node_t *node_ptr)
{
    unsigned int pos = 0;
    list_t list = *front_ptr;

    if (!front_ptr || !node_ptr)
        return (false);
    while (list && list != node_ptr) {
        pos++;
        list = list->next;
    }
    if (!list)
        return (false);
    return (list_del_elem_at_position(front_ptr, pos));
}
