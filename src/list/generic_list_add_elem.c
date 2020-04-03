/*
** EPITECH PROJECT, 2020
** add elem
** File description:
** generic list
*/

#include "generic_list.h"
#include <stdlib.h>

bool list_add_elem_at_front(list_t *list, void *elem)
{
    list_t new = malloc(sizeof(node_t));

    if (!new)
        return (false);
    new->next = *list;
    *list = new;
    new->value = elem;
    return (true);
}

bool list_add_elem_at_back(list_t *list, void *elem)
{
    list_t new = malloc(sizeof(node_t));
    list_t end = *list;

    if (!new)
        return (false);
    new->next = NULL;
    new->value = elem;
    if (!end)
        *list = new;
    else {
        while (end->next)
            end = end->next;
        end->next = new;
    }
    return (true);
}

bool list_add_elem_at_position(list_t *head, void *elem,
unsigned int pos)
{
    list_t new = NULL;
    list_t list = *head;

    if (!pos)
        return (list_add_elem_at_front(head, elem));
    for (unsigned int i = 0; i < pos - 1; i++) {
        if (!list->next)
            return (false);
        list = list->next;
    }
    new = malloc(sizeof(node_t));
    if (!new)
        return (false);
    new->next = list->next;
    new->value = elem;
    list->next = new;
    return (true);
}

