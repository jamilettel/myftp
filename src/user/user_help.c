/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** help
*/

#include "myftp.h"

bool user_help(user_t *user, const char *arg)
{
    char *help_message = "The following comands are recognized: \
USER PASS CWD CDUP QUIT DELE PWD PASV PORT HELP NOOP RETR STOR LIST";

    (void)arg;
    return (user_add_reply(user, REPLY(REPLY_HELP, help_message)));
}
