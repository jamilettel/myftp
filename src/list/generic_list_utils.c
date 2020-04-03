/*
** EPITECH PROJECT, 2020
** generic list
** File description:
** utils
*/

#include "generic_list.h"
#include <stdio.h>

unsigned int list_get_size(list_t list)
{
    int i = 0;

    while (list) {
        list = list->next;
        i++;
    }
    return (i);
}

bool list_is_empty(list_t list)
{
    return (!list);
}

void list_dump(list_t list, value_displayer_t val_disp)
{
    while (list) {
        val_disp(list->value);
        list = list->next;
    }
}

