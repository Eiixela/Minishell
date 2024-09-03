/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   lexing_utils.c :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: marvin <marvin@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/08/29 01:50:30 by marvin#+##+# */
/*   Updated: 2024/08/29 01:50:30 by marvin   ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

t_line	*first_argv(char *input, size_t len, t_line *line)
{
	line->argv = malloc(sizeof(t_argv));
	if (!line->argv)
		return (NULL);
	line->argv_head = line->argv;
	line->argv->prev = NULL;
	line->argv->next = NULL;
	line->argv->node_index = line->argc;
	line->argv->node = ft_substr(input, 0, len);
	if (!line->argv->node)
		return (NULL);
	return (line);
}

t_line	*make_argv_node(char *input, size_t len, t_line *line)
{
	t_argv	*next_node;

	if (!line->argc++)
	{
		if (!first_argv(input, len, line))
			return (NULL);
	}
	else
	{
		next_node = malloc(sizeof(t_argv));
		if (!next_node)
			return (NULL);
		next_node->prev = line->argv;
		line->argv->next = next_node;
		line->argv = next_node;
		line->argv->next = NULL;
		line->argv->node_index = line->argc;
		line->argv->node = ft_substr(input, 0, len);
		if (!line->argv->node)
			return (NULL);
	}
	return (line);
}

char	*fill_argv(char *input, t_line *line, size_t token_len)
{
	line = make_argv_node(input, token_len, line);
	if (!line || !line->argv)
		return (NULL);
	return (input);
}
