/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** server loop
*/

#include "myftp.h"
#include <sys/wait.h>

static bool manage_new_users(
    fd_set sets[2], user_t ***users, int tcp_socket, fd_set active_sets[2])
{
    if (FD_ISSET(tcp_socket, &sets[0])) {
        if (!accept_new_user(users, tcp_socket, active_sets))
            return (false);
    }
    return (true);
}

static void manage_user_quit(fd_set active_set[2], user_t ***users)
{
    if (!*users)
        return;
    for (int i = 0; (*users)[i]; i++) {
        if (!(*users)[i]->quit)
            continue;
        if ((*users)[i]->w_buff && (*users)[i]->w_buff[(*users)[i]->w_pos])
            continue;
        remove_user_from_array(users, i, active_set);
        i--;
    }
}

static bool manage_user_commands(user_t ***users)
{
    char *cmd = NULL;
    char *arg = NULL;

    if (!users)
        return (true);
    for (int i = 0; (*users)[i]; i++) {
        if (!contains_cmd((*users)[i]->r_buff, (*users)[i]->r_end))
            continue;
        if (!extract_cmd((*users)[i]->r_buff, &(*users)[i]->r_end, &cmd, &arg))
            return (false);
        if (!user_run_command((*users)[i], cmd, arg))
            return (false);
        free(cmd);
        cmd = NULL;
        free(arg);
        arg = NULL;
    }
    return (true);
}

static bool manage_data_transfer_processes(
    fd_set active_set[2], user_t **users, struct timeval **time)
{
    static struct timeval timeval;
    int status = 0;
    int ret = 0;

    if (!users)
        return (true);
    (timeval.tv_usec = 100000, *time = NULL);
    for (int i = 0; users[i]; i++) {
        if (!users[i]->dt_pid)
            continue;
        if ((ret = waitpid(users[i]->dt_pid, &status, WNOHANG)) == -1)
            return (false);
        if (ret && WIFEXITED(status)) {
            users[i]->dt_pid = 0;
            FD_SET(users[i]->cfd, &active_set[0]);
        } else {
            *time = &timeval;
            FD_CLR(users[i]->cfd, &active_set[0]);
        }
    }
    return (true);
}

bool run_server_loop(int tcp_socket)
{
    fd_set active_sets[2];
    fd_set sets[2];
    user_t **users = NULL;
    struct timeval *timeval = NULL;

    init_fd_set(active_sets, tcp_socket);
    while (1) {
        if (!manage_user_write_fd_set(active_sets, users) ||
            !manage_data_transfer_processes(active_sets, users, &timeval))
            return (false);
        manage_user_quit(active_sets, &users);
        (sets[0] = active_sets[0], sets[1] = active_sets[1]);
        select(FD_SETSIZE, &sets[0], &sets[1], NULL, timeval);
        if (!manage_new_users(sets, &users, tcp_socket, active_sets) ||
            !manage_user_write(sets, users) ||
            !manage_user_read(sets, &users, active_sets) ||
            !manage_user_commands(&users))
            return (false);
    }
}
