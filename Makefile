# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 17:39:49 by aljulien          #+#    #+#              #
#    Updated: 2024/05/14 17:20:30 by aljulien         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#RAJOUTER LES FLAGS DE COMPILIATION !!!!!!!!

SRCS = 	minishell.c\
		echo.c

OBJS = ${SRCS:.c=.o}
DEPS = ${SRCS:.c=.d}

# INCLUDES
INCPATH = -I inc/ -I libft/

# LIBRAIRIES
LIBFTPATH = -L libft/ -lft
LIBS = ${LIBFTPATH} ${INCPATH} -lm

# COMPILATION
NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra
LIBFT = libft/libft.a

.c.o:
		${CC} ${CFLAGS} ${INCPATH} -MMD -c $< -o ${<:.c=.o}

${NAME}:	${OBJS} ${LIBFT}
		${CC} -o ${NAME} ${CFLAGS} ${OBJS} ${LIBS} -lreadline ${LIBFT}


${LIBFT}: FORCE
	+$(MAKE) -C libft/

clean:	
		rm -f ${OBJS} ${DEPS}
		+$(MAKE) -C libft/ clean

fclean:	clean;
		+$(MAKE) -C libft/ fclean
		rm -f ${NAME}

all:	${NAME}

FORCE: 

bonus:	${NAME}

re:	fclean all

norminette:
		norminette srcs/

-include ${DEPS}

.PHONY: all clean fclean re norminette