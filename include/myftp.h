/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** main header file
*/

#ifndef MYFTP_H_
#define MYFTP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "user.h"

char *wrap_original_directory(bool change);

int make_socket(void);
bool prepare_socket(int sock, short port, struct sockaddr_in *addr);

#endif /* MYFTP_H_ */
