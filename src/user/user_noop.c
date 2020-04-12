/*
** EPITECH PROJECT, 2020
** user
** File description:
** noop
*/

#include "myftp.h"

bool user_noop(user_t *user, const char *arg)
{
    (void)arg;
    return (user_add_reply(user, REPLY(REPLY_OK, "Command okay.")));
}
