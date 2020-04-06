/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** main
*/

#include "myftp.h"
#include <string.h>

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
