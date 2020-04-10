/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** make socket
*/

#include "myftp.h"

int make_socket(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    return (sock);
}

bool prepare_socket(int sock, int ip, short port, struct sockaddr_in *addr)
{
    int tmp = 1;

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (ip != INADDR_ANY)
        addr->sin_addr.s_addr = htonl(ip);
    else
        addr->sin_addr.s_addr = INADDR_ANY;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &tmp, sizeof(int));
    if (bind(sock, (struct sockaddr *)addr, sizeof(*addr))) {
        perror("Bind failed");
        return (false);
    }
    if (listen(sock, 10)) {
        perror("Listen failed");
        return (false);
    }
    return (true);
}
