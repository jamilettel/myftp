/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user utils
*/

#include "myftp.h"

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
