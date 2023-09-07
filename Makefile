# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/09/07 09:49:52 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c \
		check_special_char_syntax.c \
		util.c \
		split_instr.c
		
OBJS = 	${SRCS:.c=.o}

TOTAL_OBJS = ${OBJS}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

ADDRESS_CHECK = 0

READLINE_PATH = lib

LDFLAGS = -L./${READLINE_PATH}/lib -lhistory -lreadline

INCLUDE = -I. -I./${READLINE_PATH}/include/readline

ifeq ($(ADDRESS_CHECK),1)
${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS} ${INCLUDE} ${LDFLAGS} -g -fsanitize=address -o $@
else
${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS} ${INCLUDE} ${LDFLAGS} -o $@
endif

%.o :%.c
	${CC} ${CFLAGS} -c -I. $< -o $@

all : 
	make -C ${READLINE_PATH}
	make ${NAME}

address_check : 
	make -C ${READLINE_PATH}
	@make ADDRESS_CHECK=1

clean:
	make -C ${READLINE_PATH} clean
	rm -f ${OBJS}

fclean: 
	make -C ${READLINE_PATH} fclean
	rm -rf ${OBJS} ${NAME}

re: 
	make fclean
	make all

.PHONY: all clean fclean re ADDRESS_CHECK
