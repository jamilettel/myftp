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

bool user_cwd(user_t *user, const char *path)
{
    char *original_directory = wrap_original_directory(false);

    if (!path)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Argument required.")));
    if (!original_directory || !user->wd || chdir(user->wd))
        return (false);
    if (chdir(path)) {
        return (user_add_reply(
                    user, REPLY(REPLY_FILE_ERR, "Directory change failed.")));
    }
    if (!user_set_wd(user) || chdir(original_directory))
        return (false);
    return (user_add_reply(user,
        REPLY(REPLY_FILE_ACTION_OK, "Requested file action okay, completed.")));
}

bool user_cdup(user_t *user, const char *not_used)
{
    reply_t *reply = NULL;

    if (not_used)
        return (user_add_reply(
                    user, REPLY(REPLY_ARG, "Too many arguments.")));
    if (!user_cwd(user, ".."))
        return (false);
    reply = list_get_elem_at_back(user->reply_list);
    if (reply->reply == REPLY_FILE_ACTION_OK) {
        reply->reply = REPLY_OK;
        free(reply->message);
        reply->message = strdup("Command okay.");
    }
    return (reply->message != NULL);
}

bool user_pwd(user_t *user, const char *not_used)
{
    if (not_used)
        return (user_add_reply(
                    user, REPLY(REPLY_ARG, "Too many arguments.")));
    if (!user->wd)
        return (false);
    return (user_add_reply(user, REPLY(REPLY_PATHNAME, user->wd)));
}
