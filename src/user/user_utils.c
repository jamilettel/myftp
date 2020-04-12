/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user utils
*/

#include "myftp.h"

bool user_set_w_buffer(user_t *user)
{
    reply_t *reply = list_get_elem_at_front(user->reply_list);

    user->w_pos = 0;
    if (!list_get_size(user->reply_list))
        return (true);
    if (user->w_buff_size < strlen(reply->message) + 7)
        user->w_buff = realloc(user->w_buff, strlen(reply->message) + 7);
    if (!user->w_buff)
        return (false);
    user->w_buff[0] = '0' + (reply->reply / 100) % 10;
    user->w_buff[1] = '0' + (reply->reply / 10) % 10;
    user->w_buff[2] = '0' + reply->reply % 10;
    user->w_buff[3] = ' ';
    strcpy(&user->w_buff[4], reply->message);
    strcpy(&user->w_buff[4 + strlen(reply->message)], "\r\n");
    free(reply->message);
    free(reply);
    list_del_elem_at_front(&user->reply_list);
    return (true);
}

bool user_add_reply(user_t *user, reply_t reply)
{
    reply_t *to_add = malloc(sizeof(*to_add));

    if (!reply.message || !to_add)
        return (false);
    *to_add = reply;
    return (list_add_elem_at_back(&user->reply_list, to_add));
}

user_t *user_init(int fd)
{
    user_t *user = malloc(sizeof(*user));

    if (!user)
        return (NULL);
    *user = (user_t){0};
    user->cfd = fd;
    if (wrap_original_directory(false)) {
        user->wd = strdup(wrap_original_directory(false));
        if (!user->wd)
            return (NULL);
    } else
        user_set_wd(user);
    return (user);
}

void user_destroy(user_t *user)
{
    close(user->cfd);
    if (user->dt_socket)
        close(user->dt_socket);
    if (user->ft_cfd)
        close(user->ft_cfd);
    if (user->name)
        free(user->name);
    if (user->passwd)
        free(user->passwd);
    free(user);
}
