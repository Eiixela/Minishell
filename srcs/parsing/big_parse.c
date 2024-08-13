/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 19:22:22 by saperrie          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/13 11:40:43 by aljulien         ###   ########.fr       */
=======
/*   Updated: 2024/07/30 17:51:01 by saperrie         ###   ########.fr       */
>>>>>>> a81b1ccc477598d27c263152b8076dd68edcd30a
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

// if heredoc limiter contains quote : cat << "H"D
// 										<< $USER
// 										<< HD
// 				don't expand, result is : $USER 

static	bool	dirty_redir(char *str)
{
	while (*str)
	{
		if (skip_redirection_operator(&str))
		{
			skip_white_spaces(&str);
			if (*str == '|')
				return (ft_putstr_fd \
			("syntax error near unexpected token `newline'\n", 2), false);
		}
		else
			str += 1;
	}
	return (true);
}

static bool	clean_input(char **str)
{
	skip_white_spaces((char **)str);
	if (!*str)
		return (false);
	if (**str == '|')
		return (ft_putstr_fd \
	("minishell: syntax error near unexpected token `|'\n", 2), \
			false);
	if (!even_quotes(*str))
		return (ft_putstr_fd("minishell: parsing error: missing quote\n", 2) \
	, false);
	if (!dirty_redir(*str))
		return (false);
	return (true);
}

bool	big_parse(t_line *line, char **input)
{
	char	*str;

	if (!*input || !input)
		return (false);
	skip_white_spaces((char **)input);
	if (!**input)
		return (false);
	str = *input;
	if (!clean_input((char **)&str))
		return (false);
	if (!lex((char *)str, line))
		return (false);
	if (!expand(line))
		return (false);
	if (!parse(line))
		return (false);
	return (true);
}
