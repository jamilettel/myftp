/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** dele command
*/

#include "myftp.h"

bool user_delete_file(user_t *user, const char *arg)
{
    if (!arg)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Argument required.")));
    if (chdir(user->wd))
        return (user_add_reply(user, REPLY(REPLY_FILE_ERR,
            "Could not delete file.")));
    if (remove(arg))
        return (user_add_reply(user, REPLY(REPLY_FILE_ERR,
            "Could not delete file.")));
    return (user_add_reply(user, REPLY(REPLY_FILE_ACTION_OK,
        "Requested file action okay, completed.")));
}
