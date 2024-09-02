/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 20:45:06 by saperrie          #+#    #+#             */
/*   Updated: 2024/09/02 10:25:43 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static	bool	first_redir(t_line *line)
{
	line->pipe->redir = malloc(sizeof(t_redir));
	if (!line->pipe->redir)
		return (false);
	line->pipe->redir_head = line->pipe->redir;
	line->pipe->redir->prev = NULL;
	line->pipe->redir->next = NULL;
	// line->pipe->redir->type = 0;
	line->pipe->redir->fd = NULL;
	return (true);
}

static bool	any_redir(t_line *line)
{
	t_redir	*next;

	next = malloc(sizeof(t_redir));
	line->pipe->redir->next = next;
	if (!next)
		return (false);
	line->pipe->redir->next->prev = line->pipe->redir;
	line->pipe->redir = line->pipe->redir->next;
	line->pipe->redir->next = NULL;
	// line->pipe->redir->type = 0;
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
		return (printf("redir into newlinecc\n"), line->exit_status = 2, false);
	//printf("\tfd: %s\n", line->pipe->redir->fd);
	return (true);
}