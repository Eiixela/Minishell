/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/23 10:15:35 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	redir_append(t_pipe *pipe)
{
	int	fd_file_append;

	if (pipe->redir->fd)
	{
		fd_file_append = open(pipe->redir->fd, O_WRONLY | O_APPEND | O_CREAT, 0777);
		if (fd_file_append == -1)
			return (0);
		if (dup2(fd_file_append, STDOUT_FILENO) == -1)
			return (0);
		close(fd_file_append);
	}
	return (1);
}

static int	redir_heredoc(t_pipe *pipe)
{
	//int fd_file_heredoc;

	if (pipe->redir->fd)
	{
		fprintf(stderr, "cc"); 
	}
	return (0);
}

static int	redirection_in_pipe(t_pipe *pipe)
{
	t_redir	*current_redir;
	int		fd;

	current_redir = pipe->redir;
	while (current_redir != NULL)
	{
		if (current_redir->type == HEREDOC)
			if (!redir_heredoc(pipe))
				return (perror("heredoc in redir_in_pipe function"), 0);
		if (current_redir->type == APPEND)
			if (!redir_append(pipe))
				return (perror("append function"), 0);
		if (current_redir->type == OUT_REDIR)
		{
			fd = open(current_redir->fd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fd == -1)
				return (perror("opening file"), 0);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (perror("dup2"), 0);
			close(fd);
		}
		else if (current_redir->type == IN_REDIR)
		{
			fd = open(current_redir->fd, O_RDONLY, 0777);
			if (fd == -1)
				return (perror("opening file"), 0);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (perror("dup2"), 0);
			close(fd);
		}
		current_redir = current_redir->next;
	}
	return (1);
}



static int	create_process(char **env, t_pipe *pipe, int input_fd, int output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
				return (perror("dup2 input_fd"), 0);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			if (dup2(output_fd, STDOUT_FILENO) == -1)
				return (perror("dup2 output_fd"), 0);
			close(output_fd);
		}
		if (pipe->redir != NULL)
		{
			if (!redirection_in_pipe(pipe))
				exit(EXIT_FAILURE);
		}
		if (execute_cmd(env, pipe))
			exit(pipe->ret_val); 
		exit(pipe->ret_val);
	}
	return (pid);
}

//TODO make struct env for ome builtins
static int	parse_and_execute_solo_buitlins(t_line *line)
{
	if (line->pipe->next != NULL)
		return (1);
	else
	{
		if (!ft_strcmp(line->pipe->arg[0], "echo"))
		return(ft_echo(line->pipe->arg), 0);
 	if (!ft_strcmp(line->pipe->arg[0], "cd"))
		return(0); //ft_cd(line->pipe->arg, env), 
	if (!ft_strcmp(line->pipe->arg[0], "pwd"))
		return (ft_pwd(line->pipe->arg), 0);
	if (!ft_strcmp(line->pipe->arg[0], "export"))
		return (printf("export\n"), 0); //ft_export()
	if (!ft_strcmp(line->pipe->arg[0], "unset"))
		return (printf("unset\n"), 0); //ft_unset()
	if (!ft_strcmp(line->pipe->arg[0], "env"))
		return (0); //ft_env(env), 0
	if (!ft_strcmp(line->pipe->arg[0], "exit"))
		return (ft_exit(line->pipe), 0);
	else
		return (1);
	return (0);
	}
	
}

static	int	_call_childs(char **env, t_line *line)
{
	int		pipe_fd[2];
	int		input_fd;
	int		status;
	t_pipe	*current;
	pid_t	pid;

	(void)pid;
	current = line->pipe;
	input_fd = 0;
	if (!parse_and_execute_solo_buitlins(line))
		return (1);
	while (current != NULL)
	{
		if (current->next != NULL)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"), 0);
			pid = create_process(env, current, input_fd, pipe_fd[1]);
			close(pipe_fd[1]);
			if (input_fd != 0)
				close(input_fd);
			input_fd = pipe_fd[0];
		}
		else
		{
			pid = create_process(env, current, input_fd, 1);
			if (input_fd != 0)
				close(input_fd);
		}
		current = current->next;
	}
	while (wait(&status) > 0)
		handle_exit_status_child(line, status);
	handle_exit_status_in_pipe(line);
	return (1);
}

int	pipex(char **env, t_line *line)
{
	if (!_call_childs(env, line))
		return (0);
	return (1);
}
