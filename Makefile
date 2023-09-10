# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/09/10 20:23:12 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c \
		check_special_char_syntax.c \
		util.c \
		split_instr.c \
		error.c \
		dollor_sign.c
		
OBJS = 	${SRCS:.c=.o}

TOTAL_OBJS = ${OBJS}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

INCLUDE = -I.

LIBDLL_DIR = dll
LIBGNL_DIR = gnl

LDFLAGS = -lreadline -L./${LIBDLL_DIR} -L./${LIBGNL_DIR} -ldll -lgnl

${NAME} : ${TOTAL_OBJS}
	@make -C ${LIBDLL_DIR} all
	@make -C ${LIBGNL_DIR} all
	${CC} ${CFLAGS} ${TOTAL_OBJS} ${INCLUDE} ${LDFLAGS} -o $@

%.o :%.c
	${CC} ${CFLAGS} -c -I. $< -o $@

all : ${NAME}

clean:
	@make -C ${LIBDLL_DIR} fclean
	@make -C ${LIBGNL_DIR} fclean
	rm -f ${OBJS}

fclean:
	make clean
	rm -rf ${NAME}

re: 
	make fclean
	make all

.PHONY: all clean fclean re ADDRESS_CHECK
