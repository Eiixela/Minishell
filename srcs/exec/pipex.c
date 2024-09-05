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

int	create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd,
				t_line *line, char *str)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (setup_io(input_fd, output_fd) == 0)
		{
			free_env(env);
			cleanup(line);
			exit(EXIT_FAILURE);
		}
		if (pipe->redir != NULL && handle_redirection(pipe, env) != 1)
		{
			free_env(env);
			cleanup_exec(line);
		}
		if (parse_builtin(pipe))
		{
			execute_builtins(env, pipe, line);
			free_env(env);
			if (str)
			{
				free(str);
				str = NULL;
			}
			cleanup_exec(line);
		}
		if (pipe->arg && pipe->arg[0])
			execute_cmd(env, pipe, line, str);
		if (str)
			free(str);
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
		pid = create_process(env, current, *input_fd, pipe_fd[1], line, str);
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
			pid = create_process(env, current, *input_fd, 1, line, str);
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

int	pipex(t_env	*env, t_line *line,	int	*status, char *str)
{
	line->pipe->ret_val = *status;
	if (!call_childs(env, line, str))
		return (0);
	return (1);
}