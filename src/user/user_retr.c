/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user retr functions
*/

#include "myftp.h"
#include <sys/stat.h>
#include <fcntl.h>

bool is_file(const char *filepath)
{
    struct stat s;

    if (stat(filepath, &s))
        return (false);
    return (S_ISREG(s.st_mode));
}

void user_retr_in_child(user_t *user, const char *arg)
{
    int fd = open(arg, O_RDONLY);
    char buffer[512];
    int size = 0;

    write_on_socket(user->cfd, "150 Here comes requested file.\r\n");
    if (fd != -1) {
        while ((size = read(fd, buffer, 511)) > 0) {
            buffer[size] = 0;
            write_on_socket(user->ft_cfd, buffer);
        }
        close(fd);
    }
    write_on_socket(user->cfd, "260 Transfer complete.\r\n");
}

bool user_retr(user_t *user, const char *arg)
{
    if (!user->dt_socket)
        return (user_add_reply(
                    user, REPLY(REPLY_CANT_OPEN_DATA_CON,
                                "Use PORT or PASV first.")));
    if (!arg)
        return (user_add_reply(user, REPLY(REPLY_ARG, "Argument required.")));
    if (!is_file(arg))
        return (user_add_reply(
                    user, REPLY(REPLY_FILE_ERR, "Could not open file.")));
    return (user_manage_process(user, arg, user_retr_in_child));
}
