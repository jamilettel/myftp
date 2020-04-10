/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** main
*/

#include "myftp.h"
#include <string.h>

int main(int ac, char **av)
{
    int tcp_socket = 0;
    struct sockaddr_in my_addr;
    int return_value = 0;
    int port = 0;

    if (!manage_arguments(ac, av, &port, &return_value))
        return (return_value);
    tcp_socket = make_socket();
    if (!wrap_original_directory(true))
        return (84);
    if (tcp_socket == -1 ||
        !prepare_socket(tcp_socket, INADDR_ANY, port, &my_addr))
        return (84);
    if (!run_server_loop(tcp_socket))
        return (84);
    free(wrap_original_directory(false));
    close(tcp_socket);
}
