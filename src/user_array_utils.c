/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user array utils
*/

#include "myftp.h"

void add_user_to_fd_sets(user_t *user, fd_set active_sets[2])
{
    FD_SET(user->cfd, &active_sets[0]);
}

void remove_user_from_fd_sets(user_t *user, fd_set active_sets[2])
{
    FD_CLR(user->cfd, &active_sets[0]);
    if (FD_ISSET(user->cfd, &active_sets[1]))
        FD_CLR(user->cfd, &active_sets[1]);
    if (user->ft_cfd) {
        FD_CLR(user->ft_cfd, &active_sets[0]);
        FD_CLR(user->ft_cfd, &active_sets[1]);
    }
}

void remove_user_from_array(user_t ***users, int nb, fd_set active_set[2])
{
    int size = 0;

    if (!*users)
        return;
    while ((*users)[size])
        size++;
    if (size < nb)
        return;
    remove_user_from_fd_sets((*users)[nb], active_set);
    user_destroy((*users)[nb]);
    memmove(&(*users)[nb], &(*users)[nb + 1], sizeof(**users) * (size - nb));
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
        for (; (*users)[size]; size++);
    (*users) = realloc(*users, sizeof(**users) * (size + 2));
    if (!(*users))
        return (false);
    (*users)[size] = new_user;
    (*users)[size + 1] = NULL;
    return (user_add_reply(
                new_user, REPLY(REPLY_READY, "Connected to my_ftp server")));
}
