/*
** EPITECH PROJECT, 2020
** myftp
** File description:
** tests for user commands
*/

#include "myftp.h"
#include <criterion/criterion.h>

user_t *make_test_user(bool logged);

Test(user_noop, basic_test)
{
    user_t *user1 = make_test_user(true);
    user_t *user2 = make_test_user(false);
    reply_t *reply = NULL;

    cr_assert(user_run_command(user1, "NOOP", NULL));
    cr_assert(user_run_command(user2, "NOOP", "random argument"));
    cr_expect(list_get_size(user1->reply_list) == 1);
    cr_expect(list_get_size(user2->reply_list) == 1);
    reply = list_get_elem_at_front(user1->reply_list);
    cr_assert(reply->reply == 200);
    reply = list_get_elem_at_front(user2->reply_list);
    cr_assert(reply->reply == 200);
}

Test(user_cwd, basic_test)
{
    user_t *user = make_test_user(true);
    reply_t *reply = NULL;

    cr_assert(user_run_command(user, "CWD", "/"));
    cr_assert(list_get_size(user->reply_list) == 1);
    reply = list_get_elem_at_front(user->reply_list);
    cr_expect(reply->reply == 250);
}

Test(user_cwd, error_cases)
{
    user_t *user = make_test_user(true);
    reply_t *reply = NULL;

    cr_assert(user_run_command(user, "CWD", NULL));
    cr_assert(list_get_size(user->reply_list) == 1);
    reply = list_get_elem_at_front(user->reply_list);
    cr_expect(reply->reply == 501);
    cr_assert(user_run_command(user, "CWD", "/asdlahjsdk/asdlkjhasdlk"));
    reply = list_get_elem_at_position(user->reply_list, 1);
    cr_expect(reply->reply == 550);
}

Test(user_cdup, basic_test)
{
    user_t *user = make_test_user(true);
    reply_t *reply = NULL;

    cr_assert(user_run_command(user, "CDUP", NULL));
    cr_assert(list_get_size(user->reply_list) == 1);
    reply = list_get_elem_at_front(user->reply_list);
    cr_expect(reply->reply == 200);
}

Test(user_cdup, error_cases)
{
    user_t *user = make_test_user(true);
    reply_t *reply = NULL;

    cr_assert(user_run_command(user, "CDUP", "too many arguments"));
    cr_assert(list_get_size(user->reply_list) == 1);
    reply = list_get_elem_at_front(user->reply_list);
    cr_expect(reply->reply == 501);
}
