/*
** EPITECH PROJECT, 2020
** del elem
** File description:
** generic list
*/

#include "generic_list.h"
#include <stdlib.h>

bool list_del_elem_at_front(list_t *list)
{
    list_t to_free = *list;

    if (list_is_empty(*list))
        return (false);
    (*list) = (*list)->next;
    free(to_free);
    return (true);
}

bool list_del_elem_at_back(list_t *list)
{
    list_t to_free = *list;

    if (list_is_empty(*list))
        return (false);
    if (!to_free->next)
        *list = NULL;
    else
        while (to_free->next->next)
            to_free = to_free->next;
    free(to_free->next);
    to_free->next=NULL;
    return (true);
}

bool list_del_elem_at_position(list_t *head, unsigned int pos)
{
    list_t list = *head;
    list_t to_free = NULL;

    if (list_is_empty(*head) || pos >= list_get_size(*head))
        return (false);
    if (!pos)
        return (list_del_elem_at_front(head));
    for (unsigned int i = 0; i < pos - 1; i++)
        list = list->next;
    if (!list->next)
        return (list_del_elem_at_back(head));
    to_free = list->next;
    list->next = list->next->next;
    free(to_free);
    return (true);
}

void list_clear(list_t *front)
{
    if (!(*front))
        return;
    if ((*front)->next)
        list_clear(&(*front)->next);
    free(*front);
    *front = NULL;
}
