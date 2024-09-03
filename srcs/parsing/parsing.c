/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   parsing.c  :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: saperrie <saperrie@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/05/07 15:55:40 by saperrie  #+##+# */
/*   Updated: 2024/09/02 22:56:30 by saperrie ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

bool	tag_arg(t_line *line)
{
	char	**cpy;
	size_t	tablen;
	size_t	i;

	tablen = ft_tablen(line->pipe->arg);
	cpy = ft_calloc(tablen + 2, sizeof(char *));
	if (!cpy)
		return (false);
	i = -1;
	if (line->pipe->arg)
	{
		while (++i < tablen)
			cpy[i] = line->pipe->arg[i];
		free(line->pipe->arg);
	}
	line->pipe->arg = cpy;
	line->pipe->arg[tablen] = ft_strdup(line->argv->node);
	if (!line->pipe->arg[tablen])
	{
		free(cpy);
		return (false);
	}
	line->pipe->arg[tablen + 1] = NULL;
	return (true);
}

bool	handle_redir(t_line *line, char	*first_redirection)
{
	char	operator;

	operator = is_redirection_operator((line->argv->node));
	if (operator == IN_REDIR \
		&& process_redir(line, IN_REDIR, first_redirection))
		;
	else if (operator == OUT_REDIR \
		&& process_redir(line, OUT_REDIR, first_redirection))
		;
	else if (operator == APPEND \
		&& process_redir(line, APPEND, first_redirection))
		;
	else if (operator == HEREDOC \
		&& process_redir(line, HEREDOC, first_redirection))
		;
	else
		return (false);
	return (true);
}

bool	handle_pipe(t_line *line, char *first_redirection)
{
	line->pipe->next = ft_calloc(1, sizeof(t_pipe));
	if (!line->pipe->next)
		return (false);
	line->pipe->next->prev = line->pipe;
	line->pipe = line->pipe->next;
	line->exit_status = 1;
	*first_redirection = 0;
	return (true);
}

bool	tag_tokens(t_line *line, char *first_redirection, int status)
{
	line = init_line_pipe(line, status);
	while (line->argv)
	{
		if (*line->argv->node == '|')
		{
			line->nm_arg++;
			if (!handle_pipe(line, first_redirection))
				return (false);
		}
		else if (is_redirection_operator(line->argv->node))
		{
			if (!handle_redir(line, first_redirection))
				return (false);
		}
		else
			if (!tag_arg(line))
				return (false);
		line->argv = line->argv->next;
	}
	return (true);
}

bool	parse(t_line *line, int status)
{
	char	first_redirection;

	first_redirection = 0;
	line->argv = line->argv_head;
	if (line->argv->node)
		if (!clean_surrounding_quotes(line))
			return (false);
	line->argv = line->argv_head;
	if (!tag_tokens(line, &first_redirection, status))
		return (false);
	line->pipe = line->pipe_head;
	line->pipe->redir = line->pipe->redir_head;
	line->argv = line->argv_head;
	return (true);
}
