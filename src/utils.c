/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** user utils
*/

#include "myftp.h"
#include <string.h>

char *wrap_original_directory(bool change)
{
    static char *original_directory = NULL;
    char buff[1024];

    if (change && getcwd(buff, 1024))
        original_directory = strdup(buff);
    return (original_directory);
}
