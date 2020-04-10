/*
** EPITECH PROJECT, 2020
** my ftp
** File description:
** PASV and PORT functions
*/

#include "myftp.h"
#include <ctype.h>

static bool make_dt_socket(
    user_t *user, int port, int ip, struct sockaddr_in *addr)
{
    int tcp_socket = make_socket();
    socklen_t len = sizeof(*addr);

    if (tcp_socket == -1)
        return (false);
    if (user->dt_socket)
        close(user->dt_socket);
    if (!prepare_socket(tcp_socket, ip, port, addr))
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
    if (!make_dt_socket(user, 0, INADDR_ANY, &addr))
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
    return (user_add_reply(user, reply));
}

static bool port_check_argument(const char *arg)
{
    int commas = 0;
    bool digit = false;

    for (int i = 0; arg[i]; i++) {
        if (isdigit(arg[i]) && !digit)
            digit = atoi(&arg[i]) < 256;
        if (arg[i] == ',' && digit) {
            commas++;
            digit = false;
        } else if (arg[i] == ',')
            return (false);
        if (isdigit(arg[i]) && !digit)
            return (false);
        if (!isdigit(arg[i]) && arg[i] != ',')
            return (false);
    }
    if (commas == 5 && digit)
        return (true);
    return (false);
}

static void get_ip_and_port(const char *arg, unsigned *ip, int *port)
{
    unsigned i = 0;

    *ip = atoi(arg) << 24;
    for (; arg[i] != ','; i++);
    *ip |= atoi(arg + (++i)) << 16;
    for (; arg[i] != ','; i++);
    *ip |= atoi(arg + (++i)) << 8;
    for (; arg[i] != ','; i++);
    *ip |= atoi(arg + (++i));
    for (; arg[i] != ','; i++);
    *port = atoi(arg + (++i)) << 8;
    for (; arg[i] != ','; i++);
    *port |= atoi(arg + (++i));
}

bool user_port(user_t *user, const char *arg)
{
    unsigned ip = 0;
    int port = 0;
    struct sockaddr_in addr;

    if (!arg)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Argument required.")));
    if (!port_check_argument(arg))
        return (user_add_reply(user, REPLY(REPLY_ARG, "Invalid argument")));
    get_ip_and_port(arg, &ip, &port);
    if (!make_dt_socket(user, port, ip, &addr))
        return (user_add_reply(
                    user, REPLY(
                        REPLY_CANT_OPEN_DATA_CON,
                        "Can't open data connection.")));
    return (user_add_reply(user, REPLY(REPLY_OK, "Command okay.")));
}
