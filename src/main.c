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
}

void remove_user_from_fd_sets(user_t *user, fd_set active_sets[2])
{
    FD_CLR(user->cfd, &active_sets[0]);
    if (FD_ISSET(user->cfd, &active_sets[1]))
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
        for (; (*users)[size]; size++);
    (*users) = realloc(*users, sizeof(**users) * (size + 2));
    if (!(*users))
        return (false);
    (*users)[size] = new_user;
    (*users)[size + 1] = NULL;
    return (user_add_reply(
                new_user, REPLY(REPLY_READY, "Connected to my_ftp server")));
}

bool manage_user_write_fd_set(fd_set active_set[2], user_t **users)
{
    bool return_value = true;

    if (!users)
        return (true);
    for (int i = 0; users[i] && return_value; i++) {
        if (users[i]->w_buff && users[i]->w_buff[users[i]->w_pos])
            continue;
        if (!list_get_size(users[i]->reply_list))
            FD_CLR(users[i]->cfd, &active_set[1]);
        else {
            return_value &= user_set_w_buffer(users[i]);
            FD_SET(users[i]->cfd, &active_set[1]);
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

bool manage_user_read(fd_set sets[2], user_t ***users, fd_set active_set[2])
{
    int len = 0;
    int size = 0;

    if (!users)
        return (true);
    for (int i = 0; (*users)[i]; i++) {
        if (!FD_ISSET((*users)[i]->cfd, &sets[0]))
            continue;
        len = USER_BUFFER_SIZE - 1 - (*users)[i]->r_end;
        if (len <= 0)
            continue;
        size = read((*users)[i]->cfd, &(*users)[i]->r_buffer[(*users)[i]->r_end], len);
        if (size < 0)
            return (false);
        if (size == 0) {
            remove_user_from_array(users, i, active_set);
            i--;
            continue;
        }
        (*users)[i]->r_end += size;
    }
    return (true);
}

bool manage_new_users(
    fd_set sets[2], user_t ***users, int tcp_socket, fd_set active_sets[2])
{
    if (FD_ISSET(tcp_socket, &sets[0])) {
        if (!accept_new_user(users, tcp_socket, active_sets))
            return (false);
    }
    return (true);
}

bool contains_cmd(const char *buffer, int end)
{
    if (end < 2)
        return (false);
    for (int i = 0; i < end - 1; i++)
        if (buffer[i] == '\r' && buffer[i + 1] == '\n')
            return (true);
    if (end >= USER_BUFFER_SIZE - 1)
        return (true);
    return (false);
}

bool extract_cmd(char *buffer, int *end, char **command, char **arg)
{
    int cmd_end = 0;
    int first_space = -1;

    for (int i = 0; i < *end - 1; i++) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            cmd_end = i + 1;
            buffer[i] = 0;
        } else if (buffer[i] == ' ' && first_space < 0) {
            first_space = i;
            buffer[i] = 0;
        }
    }
    if (cmd_end <= 0 && buffer[0] != '\r')
        return (true);
    if (!(*command = strdup(buffer)))
        return (false);
    if (first_space >= 0)
        if (!(*arg = strdup(&buffer[first_space + 1])))
            return (false);
    memmove(buffer, &buffer[cmd_end + 1], *end - (cmd_end + 1));
    *end -= cmd_end + 1;
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

int main(void)
{
    int tcp_socket = 0;
    struct sockaddr_in my_addr;

    tcp_socket = make_socket();
    if (!wrap_original_directory(true))
        return (84);
    if (tcp_socket == -1 || !prepare_socket(tcp_socket, 25565, &my_addr))
        return (84);
    if (!run_server_loop(tcp_socket))
        return (84);
    free(wrap_original_directory(false));
    close(tcp_socket);
}
