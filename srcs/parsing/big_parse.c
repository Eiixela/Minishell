/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   big_parse.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: saperrie <saperrie@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/05/08 19:22:22 by saperrie  #+##+# */
/*   Updated: 2024/09/02 23:26:45 by saperrie ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

bool	dirty_pipe(char *str, t_line *line)
{
	while (*str)
	{
		if (*str == '|')
		{
			skip_white_spaces(&str);
			if (*str == '|')
			{
				ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
				line->exit_status = 2;
				return (false);
			}
		}
		else
			str += 1;
	}
	return (true);
}

bool	dirty_redir(char *str, t_line *line)
{
	while (*str)
	{
		if (skip_redirection_operator(&str))
		{
			skip_white_spaces(&str);
			if (*str == '|')
			{
				ft_putstr_fd("bash: syntax error near unexpected token `|'\n", \
					2);
				line->exit_status = 2;
				return (false);
			}
			if (is_redirection_operator(str))
			{
				ft_putstr_fd("bash: syntax error near unexpected token `<<'\n", \
					2);
				line->exit_status = 2;
				return (false);
			}
		}
		else
			str += 1;
	}
	return (true);
}

bool	clean_input(char **str, t_line *line)
{
	skip_white_spaces((char **)str);
	if (!*str)
		return (false);
	if (**str == '|')
	{
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		line->exit_status = 2;
		return (false);
	}
	if (!even_quotes(*str))
	{
		ft_putstr_fd("bash: syntax error: missing quote\n", 2);
		line->exit_status = 2;
		return (false);
	}
	if (!dirty_redir(*str, line) || !dirty_pipe(*str, line))
		return (false);
	dirtier_redir(*str);
	return (true);
}

char	*big_parse(t_line *line, char **input, int status)
{
	char		*str;
	short		squote_mode;
	static char	*value;

	value = NULL;
	squote_mode = -1;
	if (!*input || !input)
		return (NULL);
	skip_white_spaces((char **)input);
	if (!**input)
		return (NULL);
	str = *input;
	if (!clean_input((char **)&str, line))
		return (NULL);
	str = expand(str, line, value, squote_mode);
	if (!str)
		return (NULL);
	clean_redir(str);
	if (!lex((char *)str, line))
		return (NULL);
	if (!parse(line, status))
		return (NULL);
	return (str);
}
