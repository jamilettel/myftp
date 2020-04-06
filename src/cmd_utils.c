/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** cmd utils
*/

#include "myftp.h"

bool contains_cmd(const char *buffer, int end)
{
    if (end < 2)
        return (false);
    for (int i = 0; i < end - 1; i++)
        if (buffer[i] == '\r' && buffer[i + 1] == '\n')
            return (true);
    if (end >= USER_BUFFER_SIZE - 1)
        return (true);
    return (false);
}

bool extract_cmd(char *buffer, int *end, char **command, char **arg)
{
    int cmd_end = 0;
    int first_space = -1;

    for (int i = 0; i < *end - 1; i++) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            cmd_end = i + 1;
            buffer[i] = 0;
        } else if (buffer[i] == ' ' && first_space < 0) {
            first_space = i;
            buffer[i] = 0;
        }
    }
    if (cmd_end <= 0 && buffer[0] != '\r')
        return (true);
    if (!(*command = strdup(buffer)))
        return (false);
    if (first_space >= 0 && !(*arg = strdup(&buffer[first_space + 1])))
        return (false);
    memmove(buffer, &buffer[cmd_end + 1], *end - (cmd_end + 1));
    *end -= cmd_end + 1;
    return (true);
}
