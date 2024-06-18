/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:23:12 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/18 13:41:02 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	_child_action(char **env, t_line line, size_t cmdnbr, int pipefd[2])
{
	if (line.pipe->redir->type == OUT_REDIR)
		dup2(line.pipe->redir->fd_out, STDOUT_FILENO);
	if (line.pipe->redir->type == IN_REDIR)
	{
		dup2(line.pipe->redir->fd_in, STDIN_FILENO);
		if (cmdnbr != 0)
			close(pipefd[0]);
	}
	else if (cmdnbr != 0)
		dup2(pipefd[0], STDIN_FILENO);
	if (!parse_builtin(env, line))
	{
		fprintf(stderr, "coucou");
		execute_cmd(env, line.pipe->arg);
	}
	if (cmdnbr != 0 && line.pipe->redir->filename == NULL)
		close(pipefd[0]);
}

static void	_exec_from_parent(char **env, t_line line, size_t cmdnbr, int pipefd[2])
{
	if (cmdnbr != 0)
		close(pipefd[0]);
	if (line.pipe->redir->type == IN_REDIR)
	{
		line.pipe->redir->saved_stdin = dup(0);
		dup2((line.pipe->redir->fd_in), STDIN_FILENO);
	}
	if (line.pipe->redir->type == OUT_REDIR)
	{
		line.pipe->redir->saved_stdout = dup(1);
		dup2((line.pipe->redir->fd_out), STDOUT_FILENO);
	}
	parse_builtin(env, line);
	if (line.pipe->redir->type == IN_REDIR)
	{
		close(line.pipe->redir->fd_in);
		dup2(line.pipe->redir->saved_stdin, STDIN_FILENO);
		close(line.pipe->redir->saved_stdin);
	}
	if (line.pipe->redir->type == OUT_REDIR)
	{
		close(line.pipe->redir->fd_out);
		dup2(line.pipe->redir->saved_stdout, STDOUT_FILENO);
		close(line.pipe->redir->saved_stdout);
	}
}

static void	error_on_open(t_line line, char *file)
{
	if (ft_strlen(file) > 0)
		ft_putstr_fd("minishell: : ", 2);
	else
		ft_putstr_fd("minishell: : ", 2);
	perror(file);
	if (line.pipe->redir->fd_in != -1)
		close(line.pipe->redir->fd_in);
	if (line.pipe->redir->fd_out != -1)
		close(line.pipe->redir->fd_out);
	line.pipe->redir->fd_out = -1;
	//return value program == 1
}

//TODO heredoc --> supposed to set the heredoc variable in the init function but it makes the proram seg fault is i use the init function, for now the heredoc is set at zero
static int io_open_fds(t_line line)
{
	int		heredoc = 0;

	if (heredoc)
	{
		if (line.pipe->redir->fd_in != -1)
			close(line.pipe->redir->fd_in);
		line.pipe->redir->fd_in = open(line.pipe->redir->filename, O_RDONLY);
		if (line.pipe->redir->fd_in != -1)
			return(error_on_open(line, line.pipe->redir->filename), 0);
	}
	return (1); 
}

//TODO remplacer printf command not ofund with action_command_not_found fonction
static int open_fds(t_line line)
{
	if (io_open_fds(line))
		return (1);
	if (!line.pipe->arg || !line.pipe->arg[0] || !line.pipe->arg[0][0])
		printf("command not found\n");
	if (line.pipe->redir->type == OUT_REDIR)
	{
		close(line.pipe->redir->fd_out); //free filename ?
		line.pipe->redir->type = NULLOS;
	}
	if (line.pipe->redir->type == IN_REDIR)
	{
		close(line.pipe->redir->fd_in); //free filename ?
		line.pipe->redir->type = NULLOS;
	}
	return (0);	
}

//TODO parent action function and executon from parent if command is a builtins
//TODO parent exection function
void	last_child(char **env, int pipefd[2], t_line line, size_t cmdnbr)
{
	pid_t	pid;
	
	if (!open_fds(line))
	{
		if (cmdnbr != 0)
			close(pipefd[0]);
		return ;
	}
	if (line.pipe == line.pipe_head && parse_builtin(env, line) == 1)
		return (_exec_from_parent(env, line, cmdnbr, pipefd));
	pid = fork();
	if (pid == -1)
	{	
		perror("minishell: fork");
		if (cmdnbr != 0)
			close(pipefd[0]);
	}
	else if (pid == 0)
		_child_action(env, line, cmdnbr, pipefd);
	// else if 
	//	_parent_action(env, line, pipefd);
}