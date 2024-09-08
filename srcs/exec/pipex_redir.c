/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:57:52 by ubuntu            #+#    #+#             */
/*   Updated: 2024/09/08 13:50:51 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child_io(t_io_fds *fds, int pipe_fd)
{
	if (pipe_fd > -1)
		close(pipe_fd);
	if (fds->input_fd != STDIN_FILENO)
	{
		if (dup2(fds->input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(1);
		}
		close(fds->input_fd);
	}
	if (fds->output_fd != STDOUT_FILENO)
	{
		if (dup2(fds->output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 output");
			exit(1);
		}
		close(fds->output_fd);
	}
}

int	setup_io(t_io_fds *fds)
{
	if (fds->input_fd != STDIN_FILENO)
	{
		if (dup2(fds->input_fd, STDIN_FILENO) == -1)
			return (perror("dup2 input_fd"), 0);
		close(fds->input_fd);
	}
	if (fds->output_fd != STDOUT_FILENO)
	{
		if (dup2(fds->output_fd, STDOUT_FILENO) == -1)
			return (perror("dup2 output_fd"), 0);
		close(fds->output_fd);
	}
	return (1);
}

void	apply_redirection(int *fd, int target_fd, t_line *line, t_env *env)
{
	if (*fd != target_fd)
	{
		if (dup2(*fd, target_fd) == -1)
		{
			perror("dup2");
			close(*fd);
			cleanup(line);
			free_env(env);
			exit(1);
		}
		close(*fd);
		*fd = target_fd;
	}
}

void	handle_redir_type(t_redir *redir, t_io_fds *fds,
				t_line *line, t_env *env)
{
	if (redir->type == IN_REDIR)
	{
		if (fds->input_fd != STDIN_FILENO)
			close(fds->input_fd);
		fds->input_fd = handle_input_redirection(redir->fd, line, env);
	}
	else if (redir->type == OUT_REDIR || redir->type == APPEND)
	{
		if (fds->output_fd != STDOUT_FILENO)
			close(fds->output_fd);
		fds->output_fd = handle_output_redirection(redir->fd, redir, line, env);
	}
	else if (redir->type == HEREDOC)
	{
		if (fds->input_fd != STDIN_FILENO)
			close(fds->input_fd);
		fds->input_fd = handle_heredoc(redir->fd, line, env);
	}
}

void	while_redir(t_line *line, t_pipe *pipe, t_env *env)
{
	t_redir		*redir;
	t_io_fds	fds;

	fds.input_fd = STDIN_FILENO;
	fds.output_fd = STDOUT_FILENO;
	redir = pipe->redir;
	while (redir)
	{
		handle_redir_type(redir, &fds, line, env);
		redir = redir->next;
	}
	apply_redirection(&fds.input_fd, STDIN_FILENO, line, env);
	apply_redirection(&fds.output_fd, STDOUT_FILENO, line, env);
}
