/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** destroy_user_array.c
*/

#include "myftp.h"

void destroy_user_array(user_t **users)
{
    if (!users)
        return;
    for (int i = 0; users[i]; i++) {
        user_destroy(users[i]);
    }
    free(users);
}
