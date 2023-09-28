# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/24 23:03:12 by hyungdki          #+#    #+#              #
#    Updated: 2023/09/24 23:19:18 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS =	quick_sort.c

OBJS = 	${SRCS:.c=.o}

NAME = libsort.a

TOTAL_OBJS = $(OBJS)

${NAME} : ${TOTAL_OBJS}
		@ar -rsc ${NAME} ${TOTAL_OBJS}

%.o :%.c
	@${CC} ${CFLAGS} -c -I. $< -o $@

all : ${NAME}

clean:
	rm -f $(OBJS)

fclean: 
	@$(MAKE) clean
	rm -f  ${NAME}

re: 
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
