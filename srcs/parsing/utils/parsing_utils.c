/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   parsing_utils.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: saperrie <saperrie@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/06/10 20:45:06 by saperrie  #+##+# */
/*   Updated: 2024/09/02 20:25:04 by saperrie ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

t_line	*init_line_pipe(t_line *line, int status)
{
	line->pipe = ft_calloc(1, sizeof(t_pipe));
	if (!line->pipe)
		return (false);
	line->pipe_head = line->pipe;
	line->pipe->prev = NULL;
	line->pipe->next = NULL;
	line->pipe->ret_val = status;
	line->nm_arg = 1;
	return (line);
}

size_t	ft_tablen(char **str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (false);
	while (str[i])
		i += 1;
	return (i);
}

bool	first_redir(t_line *line)
{
	line->pipe->redir = malloc(sizeof(t_redir));
	if (!line->pipe->redir)
		return (false);
	line->pipe->redir_head = line->pipe->redir;
	line->pipe->redir->prev = NULL;
	line->pipe->redir->next = NULL;
	line->pipe->redir->type = 0;
	line->pipe->redir->fd = NULL;
	return (true);
}

bool	any_redir(t_line *line)
{
	t_redir	*next;

	next = malloc(sizeof(t_redir));
	line->pipe->redir->next = next;
	if (!next)
		return (false);
	line->pipe->redir->next->prev = line->pipe->redir;
	line->pipe->redir = line->pipe->redir->next;
	line->pipe->redir->next = NULL;
	line->pipe->redir->type = 0;
	line->pipe->redir->fd = NULL;
	return (true);
}

bool	process_redir(t_line *line, char redir_operator, \
	char *first_redirection)
{
	short	offset;
	size_t	i;

	if (!(*first_redirection)++)
		first_redir(line);
	else
		any_redir(line);
	offset = redirection_offset(redir_operator);
	while (is_white_space(line->argv->node[offset]))
		offset += 1;
	line->pipe->redir->type = redir_operator;
	line->pipe->redir->fd = malloc(sizeof(char) * \
		ft_strlen(line->argv->node) - offset + 1);
	if (!line->pipe->redir->fd)
		return (false);
	i = 0;
	while (line->argv->node[offset])
		line->pipe->redir->fd[i++] = line->argv->node[offset++];
	line->pipe->redir->fd[i] = '\0';
	if (!*line->pipe->redir->fd && !line->argv->next)
		return (ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2), line->exit_status = 2, false);
	return (true);
}
