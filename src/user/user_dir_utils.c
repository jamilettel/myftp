/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user directory utils
*/

#include "myftp.h"

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
