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
    {"PASV", user_pasv},
    {"PORT", user_port},
    {"DELE", user_delete_file},
    {"LIST", user_list},
    {"RETR", user_retr},
    {"STOR", user_stor},
    {"HELP", user_help},
    {NULL, NULL}
};

const struct command_fct commands_shared[] = {
    {"QUIT", user_quit},
    {"NOOP", user_noop},
    {NULL, NULL}
};

static int user_run_logged_commands(user_t *user, char *cmd, char *arg)
{
    int cmp = 0;

    for (int i = 0; commands_logged[i].command_name; i++) {
        cmp = strcmp(cmd, commands_logged[i].command_name);
        if (!cmp && user->logged)
            return (commands_logged[i].fct(user, arg));
        else if (!cmp)
            return (user_add_reply(
                        user, REPLY(REPLY_NOT_LOGGED, USER_NOT_LOGGED)));
    }
    return (-1);
}

static int user_run_unlogged_commands(user_t *user, char *cmd, char *arg)
{
    int cmp = 0;

    for (int i = 0; commands_not_logged[i].command_name; i++) {
        cmp = strcmp(cmd, commands_not_logged[i].command_name);
        if (!cmp && !user->logged)
            return (commands_not_logged[i].fct(user, arg));
        else if (!cmp)
            return (user_add_reply(user, REPLY(REPLY_NOT_LOGGED, USER_LOGGED)));
    }
    return (-1);
}

static int user_run_shared_commands(user_t *user, char *cmd, char *arg)
{
    for (int i = 0; commands_shared[i].command_name; i++) {
        if (!strcmp(cmd, commands_shared[i].command_name))
            return (commands_shared[i].fct(user, arg));
    }
    return (-1);
}

bool user_run_command(user_t *user, char *cmd, char *arg)
{
    int res = 0;

    if (!cmd)
        return (user_add_reply(user, REPLY(REPLY_SYNTAX, "Command required.")));
    res = user_run_shared_commands(user, cmd, arg);
    if (res > -1)
        return (res != 0);
    res = user_run_logged_commands(user, cmd, arg);
    if (res > -1)
        return (res != 0);
    res = user_run_unlogged_commands(user, cmd, arg);
    if (res > -1)
        return (res != 0);
    return (user_add_reply(user, REPLY(REPLY_SYNTAX, "Command not found.")));
}
