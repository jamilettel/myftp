/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user login related functions
*/

#include "myftp.h"

bool user_check_username_passwd(user_t *user)
{
    reply_t reply;

    if (user->name && !user->passwd)
        return (user_add_reply(user, REPLY(REPLY_USERNAME_OK,
            "User name okay, need password.")));
    if (user->passwd && !user->name)
        return (user_add_reply(user, REPLY(REPLY_NEED_USERNAME,
            "Need account for login.")));
    if (!user->name || !user->passwd)
        return (false);
    if (!strcmp("Anonymous", user->name) && !strcmp("", user->passwd)) {
        reply.reply = REPLY_LOGGED_IN;
        reply.message = strdup("User logged in, proceed.");
        user->logged = true;
    } else {
        reply.reply = REPLY_NOT_LOGGED;
        reply.message = strdup("Username and/or password are not valid.");
    }
    (free(user->name), free(user->passwd));
    (user->name = NULL, user->passwd = NULL);
    return (user_add_reply(user, reply));
}

bool user_set_name(user_t *user, const char *name)
{
    if (!name)
        return (user_add_reply(user,
            REPLY(REPLY_ARG, "Argument required.")));
    user->name = strdup(name);
    if (!user->name)
        return (false);
    return (user_check_username_passwd(user));
}

bool user_set_password(user_t *user, const char *passwd)
{
    if (!passwd)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Argument required.")));
    user->passwd = strdup(passwd);
    if (!user->passwd)
        return (false);
    return (user_check_username_passwd(user));
}
