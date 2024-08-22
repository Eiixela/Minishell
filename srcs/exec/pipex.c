/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/22 12:43:32 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd, t_line *line)
{
    pid_t pid;
    int saved_output;

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
            if (!redirection_in_pipe(pipe, &saved_output))
                return (exit(EXIT_FAILURE), pid);
		}
		if (parse_builtin(pipe))
        {
		    if (!execute_builtins(env, pipe, line))
                exit(EXIT_SUCCESS);
		}
		else
        {
		    if (execute_cmd(env, pipe, line))
                exit(pipe->ret_val);
		}
	}
    return (pid);
}

int	_call_childs(t_env *env, t_line *line)
{
	int		pipe_fd[2];
	int		input_fd;
	int		status;
	t_pipe	*current;
	pid_t	pid;
    
	(void)pid;
	current = line->pipe;
	input_fd = 0;
 	if (parse_and_execute_solo_builtins(env, line) == 0)
		return (1);
	while (current != NULL)
	{
		if (current->next != NULL)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"), 0);
			pid = create_process(env, current, input_fd, pipe_fd[1], line);
			close(pipe_fd[1]);
			if (input_fd != 0)
				close(input_fd);
			input_fd = pipe_fd[0];
		}
		else
		{
			pid = create_process(env, current, input_fd, 1, line);
			if (input_fd != 0)
				close(input_fd);
		}
		current = current->next;
	}
	while (wait(&status) > 0)
		handle_exit_status_child(line, status);
	return (1);
}

int	pipex(t_env *env, t_line *line)
{
	if (!_call_childs(env, line))		
		return (0);
	return (1);
}
