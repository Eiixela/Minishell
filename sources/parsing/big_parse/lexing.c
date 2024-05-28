/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 19:08:18 by saperrie          #+#    #+#             */
/*   Updated: 2024/05/28 15:29:33 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_line	*make_t_line_argv_node(const char *input, size_t len, t_line *line)
{
	if (line->argc > 0)
		line->argv->prev = line->argv;
	line->argv = malloc(sizeof(t_argv));
	if (!line->argv)
		return (NULL);
	line->argv->node_index = line->token_index++;
	line->argc += 1;
	line->argv->av = ft_substr((const char *)input, 0, len);
	if (!line->argv->av)
		return (NULL);
	if (line->argc == 0)
		line->argv->prev = NULL;
	line->argv->next = NULL;
	return (line);
}

static char	*fill_argv(const char *input, t_line *line, size_t token_len)
{
	line = make_t_line_argv_node(input, token_len, line);
	if (!line || !line->argv)
		return (NULL);
	printf("\tAV%i: %s\n", line->argv->node_index, line->argv->av);
	return ((char *)input);
}

static	const char	*tokenise_argv(const char *input, t_line *line)
{
	const char	*ptr_cpy;

	ptr_cpy = input;
	if (skip_redirection_operator(&ptr_cpy))
		skip_white_spaces(&ptr_cpy);
	while (*ptr_cpy && !is_white_space(*ptr_cpy)
		&& !is_redirection_operator(ptr_cpy))
	{
		if (*ptr_cpy == '\'')
			ptr_cpy = skip_quote_content(ptr_cpy, '\'');
		else if (*ptr_cpy == '"')
			ptr_cpy = skip_quote_content(ptr_cpy, '"');
		else
			ptr_cpy += 1;
	}
	if (!*input)
		return (NULL);
	input = fill_argv(input, line, ptr_cpy - input);
	if (!input)
		return (NULL);
	return (ptr_cpy);
}

static bool	make_tokens(const char *input, t_line *line)
{
	line->argc = 0;
	line->token_index = 0;
	while (*input)
	{
		skip_white_spaces(&input);
		if (!*input)
			return (true);
		input = tokenise_argv(input, line);
		if (!input)
			return (false);
	}
	return (true);
}

bool	lex(const char *input, t_line *line)
{
	if (!make_tokens(input, line))
		return (printf("BAD_TOKEN\n", false));
	return (true);
}
