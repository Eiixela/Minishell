/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   lexing.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: saperrie <saperrie@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/05/08 19:08:18 by saperrie  #+##+# */
/*   Updated: 2024/08/29 19:09:17 by saperrie ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

char	*tokenise(char *ptr, t_line *line)
{
	char	*cpy;

	cpy = ptr;
	if (skip_redirection_operator(&cpy))
		skip_white_spaces(&cpy);
	if (is_redirection_operator(cpy))
		return (ft_putstr_fd("bash: syntax error near unexpected token `>>'\n",
				2), line->exit_status = 2, NULL);
	while (*cpy && !is_white_space(*cpy) && !is_redirection_operator(cpy) \
		&& *cpy != '|')
	{
		if (*cpy == '\'')
			cpy = skip_quote_content(cpy, '\'');
		else if (*cpy == '"')
			cpy = skip_quote_content(cpy, '"');
		else
			cpy += 1;
	}
	ptr = fill_argv(ptr, line, cpy - ptr);
	if (!ptr)
		return (NULL);
	return (cpy);
}

char	*which_token(char *input, t_line *line)
{
	char	*ptr;

	ptr = input;
	if (*ptr == '|')
	{
		ptr = fill_argv(input, line, 1);
		ptr += 1;
	}
	else
		ptr = tokenise(ptr, line);
	if (!ptr)
		return (NULL);
	return (ptr);
}

bool	make_tokens(char *input, t_line *line)
{
	line->argc = 0;
	while (*input)
	{
		skip_white_spaces(&input);
		if (!*input)
			return (true);
		input = which_token(input, line);
		if (!input)
			return (false);
	}
	return (true);
}

bool	lex(char *input, t_line *line)
{
	if (!make_tokens(input, line))
		return (false);
	return (true);
}
