/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user store functionsstore
*/

#include "myftp.h"
#include <fcntl.h>

bool is_valid_path_to_file(const char *filepath)
{
    int len = strlen(filepath);

    if (len == 0)
        return (false);
    return (filepath[len - 1] != '/');
}

bool can_write_file(const char *filepath)
{
    if (!is_valid_path_to_file(filepath))
        return (false);
    if (!access(filepath, F_OK))
        return (access(filepath, R_OK) == 0);
    return (true);
}

void user_stor_in_child(user_t *user, const char *arg)
{
    int fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    char buffer[512];
    int size = 0;

    write_on_fd(user->cfd, "150 Connection established.\r\n");
    if (fd != -1) {
        while ((size = read(user->dt_cfd, buffer, 511)) > 0) {
            buffer[size] = 0;
            write_on_fd(fd, buffer);
        }
        close(fd);
    }
    if (size == -1)
        write_on_fd(user->cfd, "426 Failure reading network stream.\r\n");
    else
        write_on_fd(user->cfd, "226 Transfer complete.\r\n");
}

bool user_stor(user_t *user, const char *arg)
{
    if (!user->dt_socket)
        return (user_add_reply(
                    user, REPLY(REPLY_CANT_OPEN_DATA_CON,
                                "Use PORT or PASV first.")));
    if (!arg)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Argument required.")));
    if (!can_write_file(arg))
        return (user_add_reply(
                    user, REPLY(REPLY_FILE_ERR, "Could not open file.")));
    return (user_manage_process(user, arg, user_stor_in_child));
}
