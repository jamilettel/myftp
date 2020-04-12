/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** cmdutils tests
*/

#include <criterion/criterion.h>
#include "myftp.h"

Test(contains_cmd, basic_test)
{
    char *buffer = "CWD directory\r\n";
    char *buffer2 = "CWD  directory\r";
    int len = strlen(buffer);

    cr_expect(contains_cmd(buffer, len));
    cr_expect(!contains_cmd(buffer2, len));
}

Test(contains_cmd, multiple_cmds_test)
{
    char *buffer = "CWD directory\r\nThis is another command\r\n";
    int len = strlen(buffer);

    cr_expect(contains_cmd(buffer, len));
}

Test(extract_cmd, basic_tests)
{
    char buffer[] = "This is a command\r\n";
    int end = strlen(buffer);
    char *cmd = NULL;
    char *arg = NULL;

    cr_assert(extract_cmd(buffer, &end, &cmd, &arg));
    cr_expect(end == 0);
    cr_expect_str_eq(cmd, "This");
    cr_expect_str_eq(arg, "is a command");
}

Test(extract_cmd, multiple_commands)
{
    char buffer[] = "This is a command\r\nThis is another command\r\n";
    int end = strlen(buffer);
    char *cmd = NULL;
    char *arg = NULL;

    cr_assert(extract_cmd(buffer, &end, &cmd, &arg));
    cr_expect_eq(end, strlen("This is another command\r\n"));
    printf("%d %lu\n", end, strlen("This is another command\r\n"));
    cr_expect_str_eq(buffer, "This is another command\r\n");
    cr_expect_str_eq(cmd, "This");
    cr_expect_str_eq(arg, "is a command");
}
