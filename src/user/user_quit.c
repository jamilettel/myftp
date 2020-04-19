/*
** EPITECH PROJECT, 2020
** user
** File description:
** quit
*/

#include "myftp.h"

bool user_quit(user_t *user, const char *arg)
{
    if (arg)
        return (user_add_reply(
                    user, REPLY(REPLY_ARG, "Too many arguments.")));
    user->quit = true;
    return (user_add_reply(user, REPLY(REPLY_QUIT, "Goodbye.")));
}
