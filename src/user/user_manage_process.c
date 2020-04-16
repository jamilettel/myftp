/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user manage process
*/

#include "myftp.h"

void write_on_fd(int fd, char *str)
{
    int len = strlen(str);
    int ret = 0;

    while (len) {
        ret = write(fd, str, len);
        if (ret == -1)
            return;
        str += ret;
        len = strlen(str);
    }
}

static void accept_and_launch_fct(
    user_t *user, const char *arg, void (*fct)(user_t *, const char *))
{
    fd_set active_set;
    struct timeval timeval;
    struct sockaddr_in addr;
    socklen_t sock_size = sizeof(struct sockaddr_in);

    timeval.tv_usec = 0;
    timeval.tv_sec = 10;
    FD_ZERO(&active_set);
    FD_SET(user->dt_socket, &active_set);
    if (select(FD_SETSIZE, &active_set, NULL, NULL, &timeval) == -1)
        return;
    if (!FD_ISSET(user->dt_socket, &active_set)) {
        write_on_fd(user->cfd, "425 Could not establish connection.\r\n");
        return;
    }
    user->dt_cfd= accept(user->dt_socket, (struct sockaddr *)&addr, &sock_size);
    if (user->dt_cfd == -1)
        write_on_fd(user->cfd, "425 Could not establish connection.\r\n");
    else
        fct(user, arg);
}

static void connect_and_launch_fct(
    user_t *user, const char *arg, void (*fct)(user_t *, const char *))
{
    socklen_t size = sizeof(struct sockaddr_in);

    user->dt_cfd = user->dt_socket;
    if (connect(user->dt_socket, (struct sockaddr *)&user->active_addr, size))
        write_on_fd(user->cfd, "425 Could not establish connection.\r\n");
    else
        fct(user, arg);
}

bool user_manage_process(
    user_t *user, const char *arg, void (*fct)(user_t *, const char *))
{
    int child;

    if (chdir(user->wd))
        return (user_add_reply(user, REPLY(
            REPLY_FILE_ERR, "Could not access current working directory.")));
    child = fork();
    if (child == -1)
        return (false);
    else if (child) {
        user->dt_pid = child;
        close(user->dt_socket);
        user->dt_socket = 0;
        return (true);
    }
    if (user->actif)
        connect_and_launch_fct(user, arg, fct);
    else
        accept_and_launch_fct(user, arg, fct);
    exit(0);
}
