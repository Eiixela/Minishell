/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:23:19 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/14 15:29:46 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	_actions_child(t_line line, int pipe_fd[2])
{
	close(pipefd[0]);
	if (line)
}

static void _action_parent(char **env, t_line line, size_t cmdnbr, int pipefd[2])
{
	close (pipefd[1]);
	if (line.pipe->redir->type == OUT_REDIR)
		close(line.pipe->redir->fd_out);
	if (line.pipe->redir->type == IN_REDIR)
		close(line.pipe->redir->fd_in);
}

int	first_child(char **env, int	pipefd[2], t_line line, size_t cmdnbr)
{
	pid_t	pid;

	(void)line;
	(void)env;
	cmdnbr = 0;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe "), 0);
	if (!open_fds(env, line, cmdnbr))
	{
		close (pipefd[1]);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		return (perror("minishell: fork"), 0);
	}
	else if (pid == 0)
		_action_child(env, line, pipefd, cmdnbr);
	else
		_action_parent(env, line, cmdnbr, pipefd);
	return (1);
}