/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** argument management functions
*/

#include "myftp.h"
#include <ctype.h>
#include <errno.h>

static bool str_is_num(const char *str)
{
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i]))
            return (false);
    }
    return (true);
}

static void print_help(int fd)
{
    dprintf(fd, "USAGE:\t./myftp port path\n\t\
port is the port number on which the server socket listens\n\t\
path is the path to the home directory for the Anonymous user\n");
}

static bool get_port(char *port_str, int *port, int *return_value)
{
    *port = atoi(port_str);
    if (*port > 65535 || *port < 1025) {
        *return_value = 84;
        dprintf(2, "Port should be between 1024 and 65536.\n");
        return (false);
    }
    return (true);
}

static bool check_directory(char *dir, int *return_value)
{
    if (chdir(dir)) {
        *return_value = 84;
        dprintf(2, "Error with directory '%s': %s.\n", dir, strerror(errno));
        return (false);
    }
    return (true);
}

bool manage_arguments(int ac, char **av, int *port, int *return_value)
{
    if (ac == 2 && (!strcmp(av[1], "-h") || !strcmp(av[1], "-help"))) {
        print_help(1);
        *return_value = 0;
        return (false);
    }
    if (ac != 3 || !str_is_num(av[1])) {
        *return_value = 84;
        print_help(2);
        return (false);
    }
    if (!get_port(av[1], port, return_value))
        return (false);
    if (!check_directory(av[2], return_value))
        return (false);
    return (true);
}
