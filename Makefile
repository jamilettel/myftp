##
## EPITECH PROJECT, 2020
## myftp
## File description:
## Makefile
##

NAME	=	myftp

SRC	=	\
		src/server_loop.c	\
		src/make_socket.c	\
		src/users_read_write.c	\
		src/main.c	\
		src/list/generic_list_del_elem.c	\
		src/list/generic_list_add_elem.c	\
		src/list/generic_list_utils.c	\
		src/list/generic_list.c	\
		src/list/generic_list_get_elem.c	\
		src/user_array_utils.c	\
		src/user/user_dir_utils.c	\
		src/user/user_utils.c	\
		src/user/user_run_commands.c	\
		src/user/user_login_utils.c	\
		src/utils.c	\
		src/cmd_utils.c	\
		src/manage_arguments.c	\
		src/user/user_quit.c	\
		src/user/user_noop.c	\
		src/user/user_dt_socket.c	\
		src/user/user_dele.c	\

OBJ	=	$(SRC:%.c=%.o)

CFLAGS	=	-W -Wall -Wextra -Wshadow		\
			-Iinclude

RM	=	rm -f

CC	=	gcc


all:	$(NAME)

$(NAME):	$(OBJ)
			$(CC) -o $(NAME) $(OBJ)

clean:
			$(RM) $(OBJ)

fclean:	clean
			$(RM) $(NAME)

re:	fclean all

.PHONY: clean fclean re
