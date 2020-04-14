/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** list
*/

#include "myftp.h"

static char *list_get_command(const char *arg)
{
    char *cmd = NULL;
    int size = 0;

    if (!arg)
        return (strdup("ls -l"));
    if (access(arg, F_OK))
        return (NULL);
    snprintf(NULL, 0, "ls -l %s%n", arg, &size);
    cmd = malloc(sizeof(char) * size + 1);
    if (cmd)
        sprintf(cmd, "ls -l %s", arg);
    return (cmd);
}

static void write_result_on_socket(int fd, FILE *file)
{
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), file))
        write_on_fd(fd, buffer);
}

void user_list_in_child(user_t *user, const char *arg)
{
    char *cmd = list_get_command(arg);
    FILE *file = NULL;

    write_on_fd(user->cfd, "150 Here comes the directory listing.\r\n");
    if (cmd) {
        file = popen(cmd, "r");
        if (file) {
            write_result_on_socket(user->ft_cfd, file);
            pclose(file);
        }
        free(cmd);
    }
    write_on_fd(user->cfd, "226 Directory send OK.\r\n");
}

bool user_list(user_t *user, const char *arg)
{
    if (!user->dt_socket)
        return (user_add_reply(
                    user, REPLY(REPLY_CANT_OPEN_DATA_CON,
                                "Use PORT or PASV first.")));
    return (user_manage_process(user, arg, user_list_in_child));
}
