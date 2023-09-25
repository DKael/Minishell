# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/09/25 20:04:48 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c \
		cd_util.c \
		cd1.c \
		cd2.c \
		check_syntax_error_and_or.c \
		check_syntax_error_dollor_braces.c \
		check_syntax_error_lts_gts.c \
		check_syntax_error_parentheses1.c \
		check_syntax_error_parentheses2.c \
		check_syntax_error_parentheses3.c \
		check_syntax_error_parentheses4.c \
		check_syntax_error.c \
		child.c \
		data_init1.c \
		data_init2.c \
		delete_functions.c \
		dll_argument_functions.c \
		dollor_sign1.c \
		dollor_sign2.c \
		echo.c \
		env.c \
		error.c \
		execute_builtin.c \
		exit.c \
		export.c \
		heredoc1.c \
		heredoc2.c \
		heredoc3.c \
		make_path.c \
		minishell.c \
		pwd.c \
		redirect1.c \
		redirect2.c \
		redirect3.c \
		signal.c \
		split_cmd1.c \
		split_cmd2.c \
		split_cmd3.c \
		split_cmd4.c \
		split_cmd5.c \
		srt_functions.c \
		unset.c \
		util1.c \
		util2.c \
		util3.c \
		util4.c \
		wildcard1.c \
		wildcard2.c \
		wildcard3.c \
		wildcard4.c \
		
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
