/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** functions that manage user reads and writes
*/

#include "myftp.h"

void init_fd_set(fd_set active_sets[2], int tcp_socket)
{
    FD_ZERO(&active_sets[0]);
    FD_ZERO(&active_sets[1]);
    FD_SET(tcp_socket, &active_sets[0]);
}

bool manage_user_write_fd_set(fd_set active_set[2], user_t **users)
{
    bool return_value = true;

    if (!users)
        return (true);
    for (int i = 0; users[i] && return_value; i++) {
        if (users[i]->w_buff && users[i]->w_buff[users[i]->w_pos])
            continue;
        if (!list_get_size(users[i]->reply_list)) {
            FD_CLR(users[i]->cfd, &active_set[1]);
            FD_SET(users[i]->cfd, &active_set[0]);
        } else {
            return_value &= user_set_w_buffer(users[i]);
            FD_SET(users[i]->cfd, &active_set[1]);
            FD_CLR(users[i]->cfd, &active_set[0]);
        }
    }
    return (return_value);
}

bool manage_user_write(fd_set sets[2], user_t **users)
{
    int pos = 0;
    int len = 0;
    int size = 0;

    if (!users)
        return (true);
    for (int i = 0; users[i]; i++) {
        if (FD_ISSET(users[i]->cfd, &sets[1])) {
            pos = users[i]->w_pos;
            len = strlen(&users[i]->w_buff[pos]);
            len = len > 256 ? 256 : len;
            size = write(users[i]->cfd, &users[i]->w_buff[pos], len);
            users[i]->w_pos+= size;
        }
        if (users[i]->w_pos == -1)
            return (false);
    }
    return (true);
}

bool manage_user_read(fd_set sets[2], user_t ***users, fd_set active_set[2])
{
    int len = 0;
    int size = 0;

    if (!users)
        return (true);
    for (int i = 0; (*users)[i]; i++) {
        if (!FD_ISSET((*users)[i]->cfd, &sets[0]))
            continue;
        if ((len = USER_BUFFER_SIZE - 1 - (*users)[i]->r_end) <= 0)
            continue;
        size = read(
            (*users)[i]->cfd, &(*users)[i]->r_buff[(*users)[i]->r_end], len);
        if (size < 0)
            return (false);
        if (size == 0) {
            remove_user_from_array(users, i, active_set);
            i--;
        } else
            (*users)[i]->r_end += size;
    }
    return (true);
}
