/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** main
*/

#include "myftp.h"
#include <string.h>

void init_fd_set(fd_set active_sets[2], int tcp_socket)
{
    FD_ZERO(&active_sets[0]);
    FD_ZERO(&active_sets[1]);
    FD_SET(tcp_socket, &active_sets[0]);
}

void add_user_to_fd_sets(user_t *user, fd_set active_sets[2])
{
    FD_SET(user->cfd, &active_sets[0]);
    FD_SET(user->cfd, &active_sets[1]);
}

void remove_user_from_fd_sets(user_t *user, fd_set active_sets[2])
{
    FD_CLR(user->cfd, &active_sets[0]);
    FD_CLR(user->cfd, &active_sets[1]);
    if (user->ft_fd) {
        FD_CLR(user->ft_fd, &active_sets[0]);
        FD_CLR(user->ft_fd, &active_sets[1]);
    }
}

bool accept_new_user(user_t ***users, int tcp_socket, fd_set active_sets[2])
{
    int size = 0;
    user_t *new_user = user_init(tcp_socket);
    socklen_t sock_size = sizeof(struct sockaddr_in);

    if (!new_user)
        return (false);
    new_user->cfd =
        accept(tcp_socket, (struct sockaddr *)&new_user->addr, &sock_size);
    if (new_user->cfd < 0)
        return (false);
    add_user_to_fd_sets(new_user, active_sets);
    if (*users)
        for (; users[size]; size++);
    (*users) = realloc(*users, sizeof(**users) * (size + 2));
    if (!(*users))
        return (false);
    (*users)[size] = new_user;
    (*users)[size + 1] = NULL;
    return (user_add_reply(new_user,
        (reply_t){REPLY_READY, strdup("Connected to my_ftp server")}));
}

bool run_server_loop(int tcp_socket)
{
    fd_set active_sets[2];
    fd_set sets[2];
    user_t **users = NULL;

    init_fd_set(active_sets, tcp_socket);
    while (1) {
        sets[0] = active_sets[0];
        sets[1] = active_sets[1];
        if (select(FD_SETSIZE, &sets[0], &sets[1], NULL, NULL) == -1)
            return (false);
        if (FD_ISSET(tcp_socket, &sets[0])) {
            if (!accept_new_user(&users, tcp_socket, active_sets))
                return (false);
        }
    }
    return (true);
}

int main(void)
{
    int tcp_socket = 0;
    struct sockaddr_in my_addr;

    tcp_socket = make_socket();
    if (!wrap_original_directory(true))
        return (84);
    if (tcp_socket == -1 || !prepare_socket(tcp_socket, 25565, &my_addr))
        return (84);
    run_server_loop(tcp_socket);
    free(wrap_original_directory(false));
    close(tcp_socket);
}
