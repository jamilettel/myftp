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

bool contains_cmd(const char *buffer, int end);
bool extract_cmd(char *buffer, int *end, char **command, char **arg);

void add_user_to_fd_sets(user_t *user, fd_set active_sets[2]);
void remove_user_from_fd_sets(user_t *user, fd_set active_sets[2]);
void remove_user_from_array(user_t ***users, int nb, fd_set active_set[2]);
bool accept_new_user(user_t ***users, int tcp_socket, fd_set active_sets[2]);

bool manage_user_commands(fd_set active_sets[2], user_t **users);
bool run_server_loop(int tcp_socket);

void init_fd_set(fd_set active_sets[2], int tcp_socket);
bool manage_user_write_fd_set(fd_set active_set[2], user_t **users);
bool manage_user_write(fd_set sets[2], user_t **users);
bool manage_user_read(fd_set sets[2], user_t ***users, fd_set active_set[2]);

bool manage_arguments(int ac, char **av, int *port, int *return_value);

#endif /* MYFTP_H_ */
