/*
** EPITECH PROJECT, 2020
** test
** File description:
** user run commands
*/

#include "myftp.h"
#include <criterion/criterion.h>

user_t *make_test_user(bool logged)
{
    user_t *user = NULL;

    if (!wrap_original_directory(false))
        wrap_original_directory(true);
    user = user_init(-1);
    user->logged = logged;
    return (user);
}

Test(user_run_command, basic_test)
{
    user_t *user = make_test_user(true);
    reply_t *reply;

    cr_assert(user_run_command(user, "PWD", NULL));
    cr_expect(list_get_size(user->reply_list) == 1);
    reply = user->reply_list->value;
    cr_expect(reply->reply == 257);
}

Test(user_run_command, not_logged_error)
{
    user_t *user = make_test_user(false);
    reply_t *reply;

    cr_assert(user_run_command(user, "PWD", NULL));
    cr_expect(list_get_size(user->reply_list) == 1);
    reply = user->reply_list->value;
    cr_expect(reply->reply == 530);
}

Test(user_run_command, login)
{
    user_t *user = make_test_user(false);
    reply_t *reply;

    cr_assert(user_run_command(user, "USER", "Anonymous"));
    cr_expect(list_get_size(user->reply_list) == 1);
    reply = user->reply_list->value;
    cr_expect(reply->reply == 331);
    cr_expect_str_eq(user->name, "Anonymous");
    cr_assert(user_run_command(user, "PASS", NULL));
    cr_expect(list_get_size(user->reply_list) == 2);
    reply = user->reply_list->next->value;
    cr_expect(reply->reply == 230);
    cr_expect(user->logged == true && !user->name && !user->passwd);
}

Test(user_run_command, login_2)
{
    user_t *user = make_test_user(false);
    reply_t *reply;

    cr_assert(user_run_command(user, "PASS", ""));
    cr_expect(list_get_size(user->reply_list) == 1);
    reply = user->reply_list->value;
    cr_expect(reply->reply == 332);
    cr_expect_str_eq(user->passwd, "");
    cr_assert(user_run_command(user, "USER", "Anonymous"));
    cr_expect(list_get_size(user->reply_list) == 2);
    reply = user->reply_list->next->value;
    cr_expect(reply->reply == 230);
    cr_expect(user->logged == true && !user->name && !user->passwd);
}
