/*
** EPITECH PROJECT, 2020
** my ftp
** File description:
** PASV and PORT functions
*/

#include "myftp.h"

static bool make_dt_pasv_socket(
    user_t *user, int port, int ip, struct sockaddr_in *addr)
{
    int tcp_socket = make_socket();
    socklen_t len = sizeof(*addr);

    if (tcp_socket == -1)
        return (false);
    if (user->dt_socket)
        close(user->dt_socket);
    if (!prepare_socket_listen(tcp_socket, ip, port, addr))
        return (false);
    if (getsockname(tcp_socket, (struct sockaddr *)addr, &len))
        return (false);
    user->dt_socket = tcp_socket;
    addr->sin_port = ntohs(addr->sin_port);
    addr->sin_addr.s_addr = ntohl(addr->sin_addr.s_addr);
    if (addr->sin_addr.s_addr == 0)
        addr->sin_addr.s_addr = 0x7f000001;
    return (true);
}

bool user_pasv(user_t *user, const char *arg)
{
    struct sockaddr_in addr;
    reply_t reply = {REPLY_PASV, 0};
    int len = 0;

    if (arg)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Too many arguments.")));
    if (!make_dt_pasv_socket(user, 0, INADDR_ANY, &addr))
        return (false);
    for (int i = 0; i < 2; i++) {
        snprintf(
            reply.message, len, "Entering Passive Mode (%d,%d,%d,%d,%d,%d).%n",
            (addr.sin_addr.s_addr & 0xff000000) / 0x1000000,
            (addr.sin_addr.s_addr & 0xff0000) / 0x10000,
            (addr.sin_addr.s_addr & 0xff00) / 0x100,
            addr.sin_addr.s_addr & 0xff, (addr.sin_port & 0xff00) / 256,
            addr.sin_port & 0xff, &len);
        if (!reply.message && !(reply.message = malloc(sizeof(char) * (len+1))))
            return (false);
    }
    user->actif = false;
    return (user_add_reply(user, reply));
}
