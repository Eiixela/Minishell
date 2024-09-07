/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 pipex.c											:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: aljulien <aljulien@student.42.fr>			+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/05/28 11:48:55 by aljulien		   #+#	  #+#			  */
/*	 Updated: 2024/09/03 11:12:49 by aljulien		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	setup_io(int input_fd, int output_fd)
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
	return (1);
}

int create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd,
    t_line *line, char *str, int pipe_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (pipe_fd > -1)
		    close(pipe_fd);
		if (input_fd != STDIN_FILENO)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input");
				exit(1);
			}
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			if (dup2(output_fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 output");
				exit(1);
			}
			close(output_fd);
		}
		t_redir *redir = pipe->redir;
		while (redir)
		{
			if (redir->type == IN_REDIR)
			{
				int	fd = open(redir->fd, O_RDONLY);
				if (fd == -1)
				{
					print_error_message("minishell: ", line->pipe->redir->fd, ": No such file or directory\n");
					free_env(env);
					cleanup(line);
					exit(1);
				}
				if (dup2(fd, STDIN_FILENO) == -1)
				{
					perror("dup2 input file");
					exit(1);
				}
				close(fd);
			}
			else if (redir->type == OUT_REDIR || redir->type == APPEND)
			{
				int flags = (redir->type == APPEND) ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC);
				int fd = open(redir->fd, flags, 0644);
				if (fd == -1)
				{
					perror("open output file");
					exit(1);
				}
				if (dup2(fd, STDOUT_FILENO) == -1)
				{
					perror("dup2 output file");
					exit(1);
				}
				close(fd);
			}
			redir = redir->next;
		}
		if (pipe->arg && pipe->arg[0])
			execute_cmd(env, pipe, line, str);
		free_env(env);
		cleanup_exec(line);
	}
	return (pid);
}



int	process_pipe(t_env *env, t_pipe	*current, int *input_fd,
				t_line *line, int cat_count, char *str)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	if (cat_count > 0)
	{
		pid = handle_cat_process(pipe_fd, line);
		(cat_count)--;
	}
	else
		pid = create_process(env, current, *input_fd, pipe_fd[1], line, str, pipe_fd[0]);
    close(pipe_fd[1]);
	if (*input_fd != 0)
		close(*input_fd);
	*input_fd = pipe_fd[0];
	return (pid);
}

int	process_commands(t_env *env, t_line	*line, int *input_fd, int cat_count, pid_t *last_pid, char *str)
{
	t_pipe	*current;
	pid_t	pid;

	current = line->pipe;
	while (current && current->arg && ft_strcmp(current->arg[0], "cat") == 0
		&& current->arg[1] == NULL && !current->redir)
	{
		(cat_count)++;
		current = current->next;
	}
	current = line->pipe;
	while (current != NULL)
	{
		if (current->next != NULL)
			pid = process_pipe(env, current, input_fd, line, cat_count, str);
		else
		{
			pid = create_process(env, current, *input_fd, 1, line, str, -1);
			if (*input_fd != 0)
				close(*input_fd);
		}
		*last_pid = pid;
		current = current->next;
	}

	return (1);
}

int	call_childs(t_env *env,	t_line *line, char *str)
{
	int		input_fd;
	int		cat_count;
	int		builtin_result;
	pid_t	last_pid;
	pid_t	wpid;

	input_fd = 0;
	cat_count = 0;
	last_pid = 0;
	builtin_result = parse_and_execute_solo_builtins(env, line);
	if (builtin_result != 1)
		return (builtin_result);
	if (!process_commands(env, line, &input_fd, cat_count, &last_pid, str))
		return (0);
	sigend();
	while ((wpid = wait(&line->exit_status)) > 0)
	{
		if (wpid == last_pid)
			handle_exit_status_child(line, line->exit_status, 0, &cat_count);
	}
	return (handle_remaining_processes(cat_count));
}

int redir_heredoc(t_pipe *pipe, t_env *env)
{
	char *temp_file;
	int heredoc_count = 0;
	t_redir *current_redir = pipe->redir;

	while (current_redir && current_redir->type == HEREDOC)
	{
		temp_file = gen_filename(heredoc_count);
		if (!temp_file)
			return (0);
		if (!handle_single_heredoc(current_redir->fd, temp_file, env))
		{
			free(temp_file);
			return (0);
		}
		free(current_redir->fd);
		current_redir->fd = temp_file;

		current_redir = current_redir->next;
		heredoc_count++;
	}
	if (heredoc_count > 0)
	{
		int fd_file_heredoc = open(pipe->redir->fd, O_RDONLY);
		if (fd_file_heredoc == -1)
			return (perror("error on open for reading"), 0);
		if (dup2(fd_file_heredoc, STDIN_FILENO) == -1)
		{
			close(fd_file_heredoc);
			return (perror("dup2 heredoc"), 0);
		}
		close(fd_file_heredoc);
	}
	return (1);
}

int process_heredocs(t_line *line, t_env *env)
{
	t_pipe *current_pipe = line->pipe;
	int heredoc_count = 0;

	while (current_pipe)
	{
		t_redir *current_redir = current_pipe->redir;
		while (current_redir)
		{
			if (current_redir->type == HEREDOC)
			{
				char *temp_file = gen_filename(heredoc_count);
				if (!temp_file)
					return (0);
				if (!handle_single_heredoc(current_redir->fd, temp_file, env))
				{
					free(temp_file);
					return (0);
				}
				free(current_redir->fd);
				current_redir->fd = temp_file;
				heredoc_count++;
			}
			current_redir = current_redir->next;
		}
		current_pipe = current_pipe->next;
	}
	return (1);
}


int	pipex(t_env *env, t_line *line, int *status, char *str)
{
	line->pipe->ret_val = *status;

	if (!process_heredocs(line, env))
		return (1);
	if (!call_childs(env, line, str))
		return (0);

	return (1);
}
