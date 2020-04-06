/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** server loop
*/

#include "myftp.h"

static bool manage_new_users(
    fd_set sets[2], user_t ***users, int tcp_socket, fd_set active_sets[2])
{
    if (FD_ISSET(tcp_socket, &sets[0])) {
        if (!accept_new_user(users, tcp_socket, active_sets))
            return (false);
    }
    return (true);
}

bool manage_user_commands(fd_set active_sets[2], user_t **users)
{
    char *command = NULL;
    char *arg = NULL;

    if (!users)
        return (true);
    for (int i = 0; users[i]; i++) {
        if (!contains_cmd(users[i]->r_buffer, users[i]->r_end))
            continue;
        if (!extract_cmd(users[i]->r_buffer, &users[i]->r_end, &command, &arg))
            return (false);
        if (!user_run_command(users[i], command, arg, active_sets))
            return (false);
        free(command);
        command = NULL;
        free(arg);
        arg = NULL;
    }
    return (true);
}

bool run_server_loop(int tcp_socket)
{
    fd_set active_sets[2];
    fd_set sets[2];
    user_t **users = NULL;

    init_fd_set(active_sets, tcp_socket);
    while (1) {
        if (!manage_user_write_fd_set(active_sets, users))
            return (false);
        sets[0] = active_sets[0];
        sets[1] = active_sets[1];
        if (select(FD_SETSIZE, &sets[0], &sets[1], NULL, NULL) == -1)
            return (false);
        if (!manage_new_users(sets, &users, tcp_socket, active_sets) ||
            !manage_user_write(sets, users) ||
            !manage_user_read(sets, &users, active_sets) ||
            !manage_user_commands(active_sets, users))
            return (false);
    }
    return (true);
}
