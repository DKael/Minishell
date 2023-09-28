# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/09/28 16:06:50 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	mandatory/main.c \
		mandatory/cd_util.c \
		mandatory/cd1.c \
		mandatory/cd2.c \
		mandatory/check_syntax_error_and_or.c \
		mandatory/check_syntax_error_dollor_braces.c \
		mandatory/check_syntax_error_lts_gts.c \
		mandatory/check_syntax_error_parentheses1.c \
		mandatory/check_syntax_error_parentheses2.c \
		mandatory/check_syntax_error_parentheses3.c \
		mandatory/check_syntax_error_parentheses4.c \
		mandatory/check_syntax_error.c \
		mandatory/child_parentheses1.c \
		mandatory/child_parentheses2.c \
		mandatory/child1.c \
		mandatory/child2.c \
		mandatory/data_init1.c \
		mandatory/data_init2.c \
		mandatory/delete_functions.c \
		mandatory/dll_argument_functions.c \
		mandatory/dollor_sign1.c \
		mandatory/dollor_sign2.c \
		mandatory/echo.c \
		mandatory/env.c \
		mandatory/error.c \
		mandatory/execute_builtin.c \
		mandatory/exit.c \
		mandatory/export.c \
		mandatory/heredoc1.c \
		mandatory/heredoc2.c \
		mandatory/heredoc3.c \
		mandatory/make_path.c \
		mandatory/minishell1.c \
		mandatory/minishell2.c \
		mandatory/minishell3.c \
		mandatory/minishell4.c \
		mandatory/pwd.c \
		mandatory/redirect1.c \
		mandatory/redirect2.c \
		mandatory/redirect3.c \
		mandatory/signal.c \
		mandatory/split_cmd1.c \
		mandatory/split_cmd2.c \
		mandatory/split_cmd3.c \
		mandatory/split_cmd4.c \
		mandatory/split_cmd5.c \
		mandatory/srt_functions.c \
		mandatory/unset.c \
		mandatory/util1.c \
		mandatory/util2.c \
		mandatory/util3.c \
		mandatory/util4.c \
		mandatory/util5.c \
		mandatory/waitpid_macro_functions.c \
		mandatory/wildcard1.c \
		mandatory/wildcard2.c \
		mandatory/wildcard3.c \
		mandatory/wildcard4.c \
		
OBJS = 	${SRCS:.c=.o}

SRCS_BONUS =	bonus/main_bonus.c \
				bonus/cd_util_bonus.c \
				bonus/cd1_bonus.c \
				bonus/cd2_bonus.c \
				bonus/check_syntax_error_and_or_bonus.c \
				bonus/check_syntax_error_dollor_braces_bonus.c \
				bonus/check_syntax_error_lts_gts_bonus.c \
				bonus/check_syntax_error_parentheses1_bonus.c \
				bonus/check_syntax_error_parentheses2_bonus.c \
				bonus/check_syntax_error_parentheses3_bonus.c \
				bonus/check_syntax_error_parentheses4_bonus.c \
				bonus/check_syntax_error_bonus.c \
				bonus/child_parentheses1_bonus.c \
				bonus/child_parentheses2_bonus.c \
				bonus/child1_bonus.c \
				bonus/child2_bonus.c \
				bonus/data_init1_bonus.c \
				bonus/data_init2_bonus.c \
				bonus/delete_functions_bonus.c \
				bonus/dll_argument_functions_bonus.c \
				bonus/dollor_sign1_bonus.c \
				bonus/dollor_sign2_bonus.c \
				bonus/echo_bonus.c \
				bonus/env_bonus.c \
				bonus/error_bonus.c \
				bonus/execute_builtin_bonus.c \
				bonus/exit_bonus.c \
				bonus/export_bonus.c \
				bonus/heredoc1_bonus.c \
				bonus/heredoc2_bonus.c \
				bonus/heredoc3_bonus.c \
				bonus/make_path_bonus.c \
				bonus/minishell1_bonus.c \
				bonus/minishell2_bonus.c \
				bonus/minishell3_bonus.c \
				bonus/minishell4_bonus.c \
				bonus/pwd_bonus.c \
				bonus/redirect1_bonus.c \
				bonus/redirect2_bonus.c \
				bonus/redirect3_bonus.c \
				bonus/signal_bonus.c \
				bonus/split_cmd1_bonus.c \
				bonus/split_cmd2_bonus.c \
				bonus/split_cmd3_bonus.c \
				bonus/split_cmd4_bonus.c \
				bonus/split_cmd5_bonus.c \
				bonus/srt_functions_bonus.c \
				bonus/unset_bonus.c \
				bonus/util1_bonus.c \
				bonus/util2_bonus.c \
				bonus/util3_bonus.c \
				bonus/util4_bonus.c \
				bonus/util5_bonus.c \
				bonus/waitpid_macro_functions_bonus.c \
				bonus/wildcard1_bonus.c \
				bonus/wildcard2_bonus.c \
				bonus/wildcard3_bonus.c \
				bonus/wildcard4_bonus.c \

OBJS_BONUS = ${SRCS_BONUS:.c=.o}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

INCLUDE = -I.

LIBDLL_DIR = libdll
LIBFT_DIR = libft
LIBGNL_DIR = libgnl
LIBSORT_DIR = libsort

ifeq ($(WITH_BONUS),1)
	TOTAL_OBJS = ${OBJS_BONUS}
else
	TOTAL_OBJS = ${OBJS}
endif

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
	rm -f ${OBJS} ${OBJS_BONUS}

fclean:
	make clean
	rm -rf ${NAME}

re: 
	make fclean
	make all

bonus:
	@make WITH_BONUS=1

.PHONY: all clean fclean re bonus
