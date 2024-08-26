/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 19:22:22 by saperrie          #+#    #+#             */
/*   Updated: 2024/08/23 14:20:06 by aljulien         ###   ########.fr       */
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

int no_output_syntax_error(char *s, t_line *line)
{
	size_t	i = 0;
	
	if (s[i] == ':' || s[i] == '!' || s[i] == '\n' || s[i] == '#')
	{	
		line->pipe->ret_val = 0;
		return (false);	
	}
	if (s[i] == '!')
	{
		line->pipe->ret_val = 1;
		return (false);
	}
	return (true);
}

bool	big_parse(t_line *line, char **input, t_env *env, int *status)
{
	char	*str;

	if (!*input || !input)
		return (false);
	if (!no_output_syntax_error(*input, line))
	{
		*status = 1;	
		return (false);
	}
	skip_white_spaces((char **)input);
	if (!**input)
		return (false);
	str = *input;
	if (!clean_input((char **)&str))
		return (false);
	str = expand(str, line);
	if (!str)
		return (false);
	if (!lex((char *)str, line))
		return (false);
  str = NULL;
  if (str)
	  free(str);
	if (!parse(line))
		return (false);
	return (true);
}
