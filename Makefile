##
## EPITECH PROJECT, 2020
## myftp
## File description:
## Makefile
##

NAME	=	myftp

MAIN	=	src/main.c	\

SRC	=	\
		src/server_loop.c					\
		src/make_socket.c					\
		src/users_read_write.c				\
		src/list/generic_list_del_elem.c	\
		src/list/generic_list_add_elem.c	\
		src/list/generic_list_utils.c		\
		src/list/generic_list.c				\
		src/list/generic_list_get_elem.c	\
		src/user_array_utils.c				\
		src/user/user_dir_utils.c			\
		src/user/user_quit.c				\
		src/user/user_manage_process.c		\
		src/user/user_list.c				\
		src/user/user_utils.c				\
		src/user/user_dele.c				\
		src/user/user_run_commands.c		\
		src/user/user_login_utils.c			\
		src/user/user_noop.c				\
		src/user/user_dt_socket.c			\
		src/user/user_retr.c				\
		src/user/user_stor.c				\
		src/user/user_help.c				\
		src/manage_arguments.c				\
		src/utils.c							\
		src/destroy_user_array.c			\
		src/cmd_utils.c						\

TEST_SRC	=	\
				tests/redirect_all_std.c		\
				tests/test_cmd_utils.c			\
				tests/test_generic_list.c		\

OBJ	=	$(SRC:%.c=%.o) $(MAIN:%.c=%.o)

TEST_FLAGS	=	-lcriterion --coverage

CFLAGS	=	-W -Wall -Wextra -Wshadow		\
			-Iinclude

RM	=	rm -f

CC	=	gcc

TEST_NAME	=	unit_tests

all:	$(NAME)

$(NAME):	$(OBJ)
			$(CC) -o $(NAME) $(OBJ)

clean:
			$(RM) $(OBJ) *.gc* $(TEST_NAME)

fclean:	clean
			$(RM) $(NAME)

tests_run:	clean
			$(CC) -o $(TEST_NAME) $(CFLAGS) $(SRC) $(TEST_SRC) $(TEST_FLAGS)
			./$(TEST_NAME)
			gcovr -etests

re:		fclean all

.PHONY: clean fclean re
