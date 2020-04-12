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

char *get_filepath_parent_directory(const char *filepath)
{
    int i = strlen(filepath) - 1;
    char *parent_dir = NULL;

    while (i > 0 && filepath[i] != '/')
        i--;
    if (filepath[i] != '/')
        return (strdup("."));
    parent_dir = strdup(filepath);
    if (parent_dir) {
        parent_dir[i] = 0;
    }
    return (parent_dir);
}

bool can_write_file(const char *filepath)
{
    char *parent_directory = NULL;
    bool result = false;

    if (!is_valid_path_to_file(filepath))
        return (false);
    if (!access(filepath, W_OK))
        return (true);
    if (!access(filepath, F_OK))
        return (false);
    parent_directory = get_filepath_parent_directory(filepath);
    if (parent_directory && !access(parent_directory, W_OK))
        result = true;
    free(parent_directory);
    return (result);
}

void user_stor_in_child(user_t *user, const char *arg)
{
    int fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    char buffer[512];
    int size = 0;

    write_on_fd(user->cfd, "150 Connection established.\r\n");
    if (fd != -1) {
        while ((size = read(user->ft_cfd, buffer, 511)) > 0) {
            buffer[size] = 0;
            write_on_fd(fd, buffer);
        }
        close(fd);
    }
    write_on_fd(user->cfd, "260 Transfer complete.\r\n");
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
