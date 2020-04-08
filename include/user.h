/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user.h
*/

#ifndef USER_H_
#define USER_H_

#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "generic_list.h"

#define REPLY(code, message) (reply_t){code, strdup(message)}
#define USER_BUFFER_SIZE 4096

#define USER_NOT_LOGGED "Use USER and PASS to login."
#define USER_LOGGED "You are already logged in."

enum reply_code {
    REPLY_ERROR = -1,
    REPLY_OK = 200,
    REPLY_READY = 220,
    REPLY_QUIT = 221,
    REPLY_LOGGED_IN = 230,
    REPLY_FILE_ACTION_OK = 250,
    REPLY_PATHNAME = 257,
    REPLY_USERNAME_OK = 331,
    REPLY_NEED_USERNAME = 332,
};

typedef struct {
    enum reply_code reply;
    char *message;
} reply_t;

typedef struct {
    int cfd;
    int ft_fd;
    char *name;
    char *passwd;
    bool logged;
    char *wd;
    struct sockaddr_in addr;
    char *w_buff;
    size_t w_buff_size;
    int w_pos;
    list_t reply_list;
    char r_buff[USER_BUFFER_SIZE];
    int r_end;
    bool quit;
} user_t;

struct command_fct {
    char *command_name;
    bool (*fct)(user_t *user, const char *path);
};

/* START User functions */
user_t *user_init(int tcp_socket);
void user_destroy(user_t *user);

/* should not be used directly */
bool user_set_wd(user_t *user);
bool user_check_username_passwd(user_t *user);
bool user_add_reply(user_t *user, reply_t reply);

/* can be used directly */
bool user_cwd(user_t *user, const char *path);
bool user_cdup(user_t *user, const char *not_used);
bool user_set_name(user_t *user, const char *name);
bool user_set_password(user_t *user, const char *passwd);
bool user_pwd(user_t *user, const char *not_used);

bool user_set_w_buffer(user_t *user);

bool user_run_command(user_t *usr, char *cmd, char *arg, fd_set active_sets[2]);
bool user_quit(user_t *user, const char *arg);

/* END User functions */


#endif /* !USER_H_ */
