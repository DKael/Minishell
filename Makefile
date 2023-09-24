# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/09/24 23:17:00 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c \
		cd_util.c \
		cd1.c \
		cd2.c \
		make_path.c \
		check_syntax_error1.c \
		check_syntax_error2.c \
		util.c \
		split_cmd.c \
		error.c \
		dollor_sign.c \
		heredoc.c \
		child.c \
		redirect.c \
		exit.c \
		echo.c \
		pwd.c \
		env.c \
		unset.c \
		export.c \
		wildcard.c
		
OBJS = 	${SRCS:.c=.o}

TOTAL_OBJS = ${OBJS}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

INCLUDE = -I.

LIBDLL_DIR = libdll
LIBFT_DIR = libft
LIBGNL_DIR = libgnl
LIBSORT_DIR = libsort

LDFLAGS = -lreadline -L./${LIBDLL_DIR} -L./${LIBGNL_DIR} -L./${LIBFT_DIR} -L./${LIBSORT_DIR} -ldll -lgnl -lft -lsort

${NAME} : ${TOTAL_OBJS}
	@make -C ${LIBDLL_DIR} all
	@make -C ${LIBGNL_DIR} all
	@make -C ${LIBFT_DIR} all
	@make -C ${LIBSORT_DIR} all
	${CC} ${CFLAGS} ${TOTAL_OBJS} -g ${INCLUDE} ${LDFLAGS} -o $@

%.o :%.c
	${CC} ${CFLAGS} -c -I. $< -o $@

all : ${NAME}

clean:
	@make -C ${LIBDLL_DIR} fclean
	@make -C ${LIBGNL_DIR} fclean
	@make -C ${LIBFT_DIR} fclean
	@make -C ${LIBSORT_DIR} fclean
	rm -f ${OBJS}

fclean:
	make clean
	rm -rf ${NAME}

re: 
	make fclean
	make all

.PHONY: all clean fclean re ADDRESS_CHECK
