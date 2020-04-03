/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user.h
*/

#ifndef USER_H_
#define USER_H_

#include <stdbool.h>

enum reply_code {
    REPLY_ERROR = -1,
    REPLY_OK = 200,
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
    int control_socket_fd;
    int ftp_socket_fd;
    char *name;
    char *passwd;
    bool logged;
    char *wd;
} user_t;

/* START User functions */
user_t *user_init();
void user_destroy(user_t *user);

/* should not be used directly */
bool user_set_wd(user_t *user);
void user_check_username_passwd(user_t *user, reply_t *reply);

/* can be used directly */
bool user_cwd(user_t *user, const char *path, reply_t *reply);
bool user_cdup(user_t *user, const char *not_used, reply_t *reply);
bool user_set_name(user_t *user, const char *name, reply_t *reply);
bool user_set_password(user_t *user, const char *passwd, reply_t *reply);
bool user_pwd(user_t *user, const char *not_used, reply_t *reply);

/* END User functions */


#endif /* !USER_H_ */
