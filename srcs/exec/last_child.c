/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:23:12 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/14 12:35:07 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	_action_child(char **env, t_line line, size_t cmdnbr, int pipefd[2])
{
	int	fd_in;
	int	fd_out;
	
	if (line.pipe->redir->type == OUT_REDIR)
	{
		fd_out = open_file(line, 1);
		dup2(fd_out, STDOUT_FILENO);
	}
	if (line.pipe->redir->type == IN_REDIR)
	{
		fd_in = open_file(line, 2);
		dup2(fd_in, STDIN_FILENO);
		if (cmdnbr != 0)
			close(pipefd[0]);
	}
	else if (cmdnbr != 0)
		dup2(pipefd[0], STDIN_FILENO);
	if (parse_builtin(env, line) == 0)
		execute_cmd(env, line.pipe->arg);
	if (cmdnbr != 0 && line.pipe->redir->type)
		close(pipefd[0]);
	exit_child(env, line, cmdnbr);
}

//TODO return val, check exit codes, readline vs gnl ? Is it requiered or can you just use readline ?
static	void	_action_parent(char **env, t_line line, size_t cmdnbr, int pipefd[2])
{
	(void)env;
	(void)line;
	if (cmdnbr != 0)
		close(pipefd[0]);
/* 	line.return_val = check_exit_codes(line);
	if (line.return_val == 2)
	{
		line.return_val == 130;
		if (!use_readline())
		{
			if (cmdnbr != 0)
				close(pipefd[0]);
			close_program(???);
			exit(line.return_val);
		}
	}
	else if (line.return_val > 255)
		line.return_val = WEXITSTATUS(line.return_val); */
}

static	void	_exec_from_parent(char **env, t_line line, size_t cmdnbr, int pipefd[2])
{
	int	fd_in;
	int	fd_out;

	if (cmdnbr != 0)
		close (pipefd[0]);
	if (line.pipe->redir->type == IN_REDIR)
	{
		line.pipe->save_stdin = dup(0);
		fd_in = open_file(line, 2);
		dup2(fd_in, STDIN_FILENO);
	}
	if(line.pipe->redir->type == OUT_REDIR)
	{
		line.pipe->save_stdout = dup(1);
		fd_out = open_file(line, 1);
		dup2(fd_out, STDOUT_FILENO);
	}
	parse_builtin(env, line);
	if (line.pipe->redir->type == IN_REDIR)
	{
		close(fd_in);
		dup2(line.pipe->save_stdin, STDIN_FILENO);
		close(line.pipe->save_stdin);
	}
	if(line.pipe->redir->type == OUT_REDIR)
	{
		close (fd_out);
		dup2(line.pipe->save_stdout, STDOUT_FILENO);
		close(line.pipe->save_stdout);
	}
	
}

int	last_child(char **env, int pipefd[2], t_line line, size_t cmdnbr)
{
	if (cmdnbr == 0 && parse_builtin(env, line) == 0)
		return (_exec_from_parent(env, line, cmdnbr, pipefd), 0);
	pipefd[0] = fork();
	if (pipefd[0] == -1)
	{	
		if (cmdnbr != 0)
			close(pipefd[0]);
		return (perror("minishell: fork"), 0);
	}
	else if (pipefd[0] == 0)
		_action_child(env, line, cmdnbr, pipefd);
	else 
		_action_parent(env, line, cmdnbr, pipefd);
	return (1);
}