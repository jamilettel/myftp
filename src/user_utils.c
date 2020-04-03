/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user utils
*/

#include "myftp.h"
#include <string.h>

bool user_set_wd(user_t *user)
{
    char buff[1024];

    if (!getcwd(buff, 1024))
        return (false);
    if (user->wd)
        free(user->wd);
    user->wd = strdup(buff);
    if (!user->wd)
        return (false);
    return (true);
}

user_t *user_init(int fd)
{
    user_t *user = malloc(sizeof(*user));

    if (!user)
        return (NULL);
    *user = (user_t){0};
    user->control_socket_fd = fd;
    user_set_wd(user);
    return (user);
}

void user_destroy(user_t *user)
{
    close(user->control_socket_fd);
    if (user->ftp_socket_fd)
        close(user->ftp_socket_fd);
    if (user->name)
        free(user->name);
    if (user->passwd)
        free(user->passwd);
}

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

bool user_cwd(user_t *user, const char *path, reply_t *reply)
{
    char *original_directory = wrap_original_directory(false);

    if (!reply) {
        reply->message = strdup("CWD requires an argument.");
        reply->reply = REPLY_ERROR;
        return (reply->message != NULL);
    }
    if (!original_directory || !user->wd || chdir(user->wd))
        return (false);
    if (chdir(path)) {
        reply->message = strdup("Could not change to directory.");
        reply->reply = REPLY_ERROR;
        return (reply->message != NULL);
    }
    if (!user_set_wd(user) || chdir(original_directory))
        return (false);
    reply->message = strdup("Requested file action okay, completed.");
    reply->reply = REPLY_FILE_ACTION_OK;
    return (reply->message != NULL);
}

bool user_cdup(user_t *user, const char *not_used, reply_t *reply)
{
    if (not_used) {
        reply->message = strdup("CDUP doesn't require arguments.");
        reply->reply = REPLY_ERROR;
        return (reply->message != NULL);
    }
    if (!user_cwd(user, "..", reply))
        return (false);
    if (reply->reply == REPLY_FILE_ACTION_OK) {
        reply->reply = REPLY_OK;
        free(reply->message);
        reply->message = strdup("Command okay.");
    }
    return (reply->message != NULL);
}

bool user_pwd(user_t *user, const char *not_used, reply_t *reply)
{
    if (not_used) {
        reply->message = strdup("PWD doesn't require arguments.");
        reply->reply = REPLY_ERROR;
        return (reply->message != NULL);
    }
    if (!user->wd)
        return (false);
    reply->message = strdup(user->wd); 
    reply->reply = REPLY_PATHNAME;
    return (reply->message != NULL);
}
