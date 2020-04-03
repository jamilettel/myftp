/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user login related functions
*/

#include "myftp.h"

void user_check_username_passwd(user_t *user, reply_t *reply)
{
    if (user->name && !user->passwd) {
        reply->reply = REPLY_USERNAME_OK;
        reply->message = strdup("User name okay, need password.");
    }
    if (user->passwd && !user->name) {
        reply->reply = REPLY_NEED_USERNAME;
        reply->message = strdup("Need account for login.");
    }
    if (!user->name || !user->passwd)
        return;
    if (!strcmp("Anonymous", user->name) && !strcmp("", user->passwd)) {
        reply->reply = REPLY_LOGGED_IN;
        reply->message = strdup("User logged in, proceed.");
        user->logged = true;
    } else {
        reply->reply = REPLY_ERROR;
        reply->message = strdup("Username and/or password are not valid.");
    }
    (free(user->name), free(user->passwd));
    user->name = NULL;
    user->passwd = NULL;
}

bool user_set_name(user_t *user, const char *name, reply_t *reply)
{
    if (!name) {
        reply->message = strdup("NAME requires an argument.");
        reply->reply = REPLY_ERROR;
        return (reply->message != NULL);
    }
    user->name = strdup(name);
    if (!user->name)
        return (false);
    user_check_username_passwd(user, reply);
    return (reply->message != NULL);
}

bool user_set_password(user_t *user, const char *passwd, reply_t *reply)
{
    if (!passwd) {
        reply->message = strdup("PASS requires an argument.");
        reply->reply = REPLY_ERROR;
        return (reply->message != NULL);
    }
    user->passwd = strdup(passwd);
    if (!user->passwd)
        return (false);
    user_check_username_passwd(user, reply);
    return (reply->message != NULL);
}
