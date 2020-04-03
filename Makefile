##
## EPITECH PROJECT, 2020
## myftp
## File description:
## Makefile
##

NAME	=	myftp

SRC	=	src/main.c			\
		src/make_socket.c	\

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
