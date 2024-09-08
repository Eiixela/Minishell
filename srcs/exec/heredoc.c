/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:27:27 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/08 17:27:29 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_line(int fd, char *line, t_env *env)
{
	char	*expanded_line;
	int		result;

	expanded_line = expand_variables(line, env);
	if (expanded_line)
		result = process_expanded_line(fd, expanded_line);
	else
		result = write_to_heredoc(fd, line);
	return (result);
}

static int	process_single_heredoc(t_redir *redir, int count, t_env *env)
{
	char	*temp_file;

	redir->fd = back_to_positive(redir->fd);
	temp_file = gen_filename(count);
	if (!temp_file)
		return (0);
	if (!handle_single_heredoc(redir->fd, temp_file, env))
	{
		free(temp_file);
		return (0);
	}
	free(redir->fd);
	redir->fd = temp_file;
	return (1);
}

int	process_heredocs(t_line *line, t_env *env)
{
	int		heredoc_count;
	t_pipe	*current_pipe;
	t_redir	*current_redir;

	current_pipe = line->pipe;
	heredoc_count = 0;
	while (current_pipe)
	{
		current_redir = current_pipe->redir;
		while (current_redir)
		{
			if (current_redir->type == HEREDOC)
			{
				if (!process_single_heredoc(current_redir, heredoc_count, env))
					return (0);
				heredoc_count++;
			}
			current_redir = current_redir->next;
		}
		current_pipe = current_pipe->next;
	}
	return (1);
}

int	process_heredoc_redir(t_redir *redir, int count, t_env *env)
{
	char	*temp_file;

	temp_file = gen_filename(count);
	if (!temp_file)
		return (0);
	if (!handle_single_heredoc(redir->fd, temp_file, env))
	{
		free(temp_file);
		return (0);
	}
	free(redir->fd);
	redir->fd = temp_file;
	return (1);
}

int	redir_heredoc(t_pipe *pipe, t_env *env)
{
	int		heredoc_count;
	t_redir	*current_redir;

	heredoc_count = 0;
	current_redir = pipe->redir;
	while (current_redir && current_redir->type == HEREDOC)
	{
		if (!process_heredoc_redir(current_redir, heredoc_count, env))
			return (0);
		current_redir = current_redir->next;
		heredoc_count++;
	}
	if (heredoc_count > 0)
	{
		if (!open_and_dup_heredoc(pipe->redir->fd))
			return (0);
	}
	return (1);
}
