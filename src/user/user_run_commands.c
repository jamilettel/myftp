/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user run commands
*/

#include "myftp.h"

const struct command_fct commands[] = {
    {"USER", user_set_name},
    {"PASS", user_set_password},
    {"CWD", user_cwd},
    {"CDUP", user_cdup},
    {"PWD", user_pwd},
    {NULL, NULL}
};

bool user_run_command(user_t *user, char *cmd, char *arg, fd_set active_sets[2])
{
    if (!cmd)
        return (user_add_reply(user, REPLY(REPLY_ERROR, "Command required.")));
    for (int i = 0; commands[i].command_name; i++)
        if (!strcmp(cmd, commands[i].command_name))
            return (commands[i].fct(user, arg));
    (void)active_sets;
    return (user_add_reply(user, REPLY(REPLY_ERROR, "Command not found.")));
}
