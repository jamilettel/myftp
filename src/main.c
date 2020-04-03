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
    int tcp_socket = make_socket();
    struct sockaddr_in my_addr;

    if (!wrap_original_directory(true))
        return (84);
    if (tcp_socket == -1 || !prepare_socket(tcp_socket, 25565, &my_addr))
        return (84);
    close(tcp_socket);
}

