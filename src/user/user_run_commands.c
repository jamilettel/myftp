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

const struct command_fct commands_shared[] = {
    {"QUIT", user_quit},
    {"NOOP", user_noop},
    {NULL, NULL}
};

static int user_run_logged_commands(
    user_t *user, char *cmd, char *arg, fd_set active_sets[2])
{
    int cmp = 0;

    (void)active_sets;
    for (int i = 0; commands_logged[i].command_name; i++) {
        cmp = strcmp(cmd, commands_logged[i].command_name);
        if (!cmp && user->logged)
            return (commands_logged[i].fct(user, arg));
        else if (!cmp)
            return (user_add_reply(user, REPLY(REPLY_ERROR, USER_NOT_LOGGED)));
    }
    return (-1);
}

static int user_run_unlogged_commands(
    user_t *user, char *cmd, char *arg, fd_set active_sets[2])
{
    int cmp = 0;

    (void)active_sets;
    for (int i = 0; commands_not_logged[i].command_name; i++) {
        cmp = strcmp(cmd, commands_not_logged[i].command_name);
        if (!cmp && !user->logged)
            return (commands_not_logged[i].fct(user, arg));
        else if (!cmp)
            return (user_add_reply(user, REPLY(REPLY_ERROR, USER_LOGGED)));
    }
    return (-1);
}

static int user_run_shared_commands(
    user_t *user, char *cmd, char *arg, fd_set active_sets[2])
{
    (void)active_sets;
    for (int i = 0; commands_shared[i].command_name; i++) {
        if (!strcmp(cmd, commands_shared[i].command_name))
            return (commands_shared[i].fct(user, arg));
    }
    return (-1);
}

bool user_run_command(user_t *user, char *cmd, char *arg, fd_set active_sets[2])
{
    int res = 0;

    if (!cmd)
        return (user_add_reply(user, REPLY(REPLY_ERROR, "Command required.")));
    res = user_run_logged_commands(user, cmd, arg, active_sets);
    if (res > -1)
        return (res != 0);
    res = user_run_unlogged_commands(user, cmd, arg, active_sets);
    if (res > -1)
        return (res != 0);
    res = user_run_shared_commands(user, cmd, arg, active_sets);
    if (res > -1)
        return (res != 0);
    return (user_add_reply(user, REPLY(REPLY_ERROR, "Command not found.")));
}
