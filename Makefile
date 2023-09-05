# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/09/04 17:53:47 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c \
		check_special_char_syntax.c \
		util.c \
		
OBJS = 	${SRCS:.c=.o}

TOTAL_OBJS = ${OBJS}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

ADDRESS_CHECK = 0

ifeq ($(ADDRESS_CHECK),1)
${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS} -lreadline -g -fsanitize=address -o $@
else
${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS} -lreadline -o $@
endif

%.o :%.c
	${CC} ${CFLAGS} -c -I. $< -o $@

all : ${NAME}

address_check : 
	@make ADDRESS_CHECK=1

clean:
	rm -f ${OBJS}

fclean: 
	make clean
	rm -rf ${NAME}

re: 
	make fclean
	make all

.PHONY: all clean fclean re ADDRESS_CHECK
