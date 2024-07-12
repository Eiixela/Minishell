# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/03 01:12:35 by saperrie          #+#    #+#              #
#    Updated: 2024/07/12 09:11:19 by aljulien         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

INCLUDE_DIR = inc/
LIBFT_DIR = libft/

CFLAGS = -Wall -Wextra -Werror -g3 
IFLAGS = -I$(INCLUDE_DIR) -I$(LIBFT_DIR)
DFLAGS = -MMD -MP
LFLAGS = -L$(LIBFT_DIR) -lft -lreadline

OBJECT_DIR = .obj/

OBJECTS = $(patsubst srcs/%.c,$(OBJECT_DIR)%.o,\
		srcs/main/minishell.c\
		srcs/parsing/big_parse.c\
		srcs/parsing/lexing.c\
		srcs/parsing/expansion.c\
		srcs/parsing/parsing.c\
		srcs/parsing/path_check.c\
		srcs/parsing/utils/parsing_utils.c \
		srcs/parsing/utils/clean_quotes.c\
		srcs/parsing/utils/utils.c\
		srcs/parsing/utils/redirection_utils.c\
		srcs/builtins/ft_echo.c\
		srcs/builtins/ft_pwd.c\
		srcs/builtins/ft_cd.c\
		srcs/builtins/ft_env.c\
		srcs/builtins/ft_export.c\
		srcs/builtins/ft_exit.c\
		srcs/builtins/ft_unset.c\
		srcs/exec/pipex.c\
		srcs/exec/get_path.c\
		srcs/exec/execve.c\
		srcs/exec/parse_builtins.c\
							)

OBJ_SUBDIRS = $(sort $(dir ${OBJECTS}))

DEPENDENCIES = $(OBJECTS:.o=.d)

LIBFT = $(LIBFT_DIR)libft.a
NAME = minishell

.PHONY: all
all: $(NAME)

-include $(DEPENDENCIES)

$(NAME): $(OBJECTS) | $(LIBFT)
	$(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS) -o $@ $(OBJECTS) $(LFLAGS)

$(OBJECT_DIR)%.o: srcs/%.c | $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS) -o $@ -c $<

$(OBJECT_DIR):
	mkdir -p ${OBJ_SUBDIRS}

$(LIBFT)::
	@make --no-print-directory -C $(LIBFT_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean
	@make --no-print-directory all

