# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aljulien <aljulien@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 17:39:49 by aljulien          #+#    #+#              #
#    Updated: 2024/06/04 10:47:48 by aljulien         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#RAJOUTER LES FLAGS DE COMPILIATION !!!!!!!!

SRCDIR = sources

BUILTIN_DIR = $(SRCDIR)/builtins
MAIN_DIR = $(SRCDIR)/main
EXEC_DIR = $(SRCDIR)/exec
PARSING_DIR = $(SRCDIR)/parsing
BIGPARSE_DIR = $(PARSING_DIR)/big_parse
UTILSPARSE_DIR = $(PARSING_DIR)/utils
EXEC_DIR = $(SRCDIR)/exec


SRCS = 	$(MAIN_DIR)/minishell.c \
		$(BUILTIN_DIR)/echo.c \
		$(BUILTIN_DIR)/pwd.c\
		$(BIGPARSE_DIR)/big_parse.c\
		$(BIGPARSE_DIR)/lexing.c\
		$(BIGPARSE_DIR)/parsing.c\
		$(UTILSPARSE_DIR)/redirection_utils.c\
		$(UTILSPARSE_DIR)/utils.c\
		$(UTILSPARSE_DIR)/clean_quotes.c\
		$(EXEC_DIR)/pipex.c\
		$(EXEC_DIR)/execve.c\
		$(EXEC_DIR)/get_path.c\
		$(BIGPARSE_DIR)/expansion.c\
		$(BIGPARSE_DIR)/path_check.c

OBJS = ${SRCS:.c=.o}
DEPS = ${SRCS:.c=.d}

# INCLUDES
INCPATH = -I inc/ -I libft/

# LIBRAIRIES
LIBFTPATH = -L libft/ -lft
LIBS = ${LIBFTPATH} ${INCPATH}

# COMPILATION
NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -fsanitize=address -g3
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