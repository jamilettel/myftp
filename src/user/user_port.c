/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user port functions
*/

#include <ctype.h>
#include "myftp.h"

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

static bool make_dt_active_socket(user_t *user, int port, int ip)
{
    int tcp_socket = make_socket();

    if (tcp_socket == -1)
        return (false);
    if (user->dt_socket)
        close(user->dt_socket);
    if (!prepare_socket_connect(ip, port, &user->active_addr))
        return (false);
    user->dt_socket = tcp_socket;
    return (true);
}

bool user_port(user_t *user, const char *arg)
{
    unsigned ip = 0;
    int port = 0;

    if (!arg)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Argument required.")));
    if (!port_check_argument(arg))
        return (user_add_reply(user, REPLY(REPLY_ARG, "Invalid argument")));
    get_ip_and_port(arg, &ip, &port);
    if (!make_dt_active_socket(user, port, ip))
        return (user_add_reply(
                    user, REPLY(
                        REPLY_CANT_OPEN_DATA_CON,
                        "Can't create socket.")));
    user->actif = true;
    return (user_add_reply(user, REPLY(REPLY_OK, "Command okay.")));
}
