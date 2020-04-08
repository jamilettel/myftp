/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user run commands
*/

#include "myftp.h"

const struct command_fct commands_not_logged[] = {
    {"USER", user_set_name},
    {"PASS", user_set_password},
    {NULL, NULL}
};

const struct command_fct commands_logged[] = {
    {"CWD", user_cwd},
    {"CDUP", user_cdup},
    {"PWD", user_pwd},
    {NULL, NULL}
};

bool user_run_command(user_t *user, char *cmd, char *arg, fd_set active_sets[2])
{
    int cmp = 0;

    if (!cmd)
        return (user_add_reply(user, REPLY(REPLY_ERROR, "Command required.")));
    for (int i = 0; commands_logged[i].command_name; i++) {
        cmp = strcmp(cmd, commands_logged[i].command_name);
        if (!cmp && user->logged)
            return (commands_logged[i].fct(user, arg));
        else if (!cmp)
            return (user_add_reply(user, REPLY(REPLY_ERROR, USER_NOT_LOGGED)));
    }
    for (int i = 0; commands_not_logged[i].command_name; i++) {
        cmp = strcmp(cmd, commands_not_logged[i].command_name);
        if (!cmp && !user->logged)
            return (commands_not_logged[i].fct(user, arg));
        else if (!cmp)
            return (user_add_reply(user, REPLY(REPLY_ERROR, USER_LOGGED)));
    }
    (void)active_sets;
    return (user_add_reply(user, REPLY(REPLY_ERROR, "Command not found.")));
}
