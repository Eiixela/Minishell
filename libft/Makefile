# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/10 10:25:02 by aljulien          #+#    #+#              #
#    Updated: 2024/08/13 12:22:31 by aljulien         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = ft_atoi.c\
	ft_bzero.c\
	ft_isalnum.c\
	ft_isalpha.c\
	ft_isascii.c\
	ft_isdigit.c\
	ft_isprint.c\
	ft_memchr.c\
	ft_memcmp.c\
	ft_memcpy.c\
	ft_memmove.c\
	ft_memset.c\
	ft_strchr.c\
	ft_strlcat.c\
	ft_strlcpy.c\
	ft_strlen.c\
	ft_strncmp.c\
	ft_strnstr.c\
	ft_strrchr.c\
	ft_tolower.c\
	ft_toupper.c\
	ft_calloc.c\
	ft_itoa.c\
	ft_putchar_fd.c\
	ft_putnbr_fd.c\
	ft_putendl_fd.c\
	ft_putstr_fd.c\
	ft_striteri.c\
	ft_strjoin.c\
	ft_strmapi.c\
	ft_strtrim.c\
	ft_strdup.c\
	ft_substr.c\
	ft_split.c\
	ft_strcmp.c\
	ft_dstrlen.c\
	ft_atoll.c\
	ft_free_dtab.c\

SRC_B =	ft_lstnew_bonus.c\
	ft_lstadd_front_bonus.c\
	ft_lstadd_back_bonus.c\
	ft_lstsize_bonus.c\
	ft_lstlast_bonus.c\
	ft_lstdelone_bonus.c\
	ft_lstclear_bonus.c\
	ft_lstiter_bonus.c\
	ft_lstmap_bonus.c\

CC = cc

FLAGS = -Wall -Werror -Wextra -I.

OBJ = $(SRC:%.c=%.o)

OBJ_B = $(SRC_B:%.c=%.o)

NAME = libft.a

all: $(NAME)

$(NAME): 		$(OBJ) $(OBJ_B)
		ar rcs $(NAME) $(OBJ) $(OBJ_S)

bonus:	$(OBJ) $(OBJ_B)
			ar rcs $(NAME) $(OBJ) $(OBJ_B)

%.o : %.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
				
clean :
		rm -f $(OBJ) $(OBJ_B)

fclean : clean
		rm -f $(NAME) 

re: fclean all

.PHONY : all clean fclean re bonus 

