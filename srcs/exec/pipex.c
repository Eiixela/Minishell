/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/13 13:53:28 by aljulien         ###   ########.fr       */
=======
/*   Updated: 2024/07/30 15:36:45 by aljulien         ###   ########.fr       */
>>>>>>> a81b1ccc477598d27c263152b8076dd68edcd30a
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd)
{
    pid_t pid;
    int saved_output;

    pid = fork();
    if (pid == -1)
        return (perror("fork"), 0);
    if (pid == 0)
    {
<<<<<<< HEAD
        if (input_fd != STDIN_FILENO)
=======
        line_heredoc = readline("> ");
        if (ft_strcmp(line_heredoc, pipe->redir->fd) == 0)
            return (1);
    }
	return (0);
}

static int redirection_in_pipe(t_pipe *pipe, int *saved_output)
{
    t_redir *current_redir;
    t_redir *last_out_redir = NULL;
    int     fd;
    int     flags;

    *saved_output = dup(STDOUT_FILENO);
    if (*saved_output == -1)
        return (perror("dup"), 0);
    current_redir = pipe->redir;
    while (current_redir != NULL)
    {
        if (current_redir->type == OUT_REDIR || current_redir->type == APPEND)
>>>>>>> a81b1ccc477598d27c263152b8076dd68edcd30a
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
            if (!execute_builtins(env, pipe))
                exit(EXIT_SUCCESS);
        }
        else
        {
            if (execute_cmd(env, pipe))
                exit(pipe->ret_val);
        }
<<<<<<< HEAD
=======
        if (!ft_strcmp(line->pipe->arg[0], "echo"))
            ft_echo(line->pipe->arg); // Directly call the echo function
        else if (!ft_strcmp(line->pipe->arg[0], "cd"))
            ; // Implement cd logic
        else if (!ft_strcmp(line->pipe->arg[0], "pwd"))
            ft_pwd(line->pipe->arg);
        else if (!ft_strcmp(line->pipe->arg[0], "export"))
            printf("export\n");
        else if (!ft_strcmp(line->pipe->arg[0], "unset"))
            printf("unset\n");
        else if (!ft_strcmp(line->pipe->arg[0], "env"))
            ; // Implement env logic
        else if (!ft_strcmp(line->pipe->arg[0], "exit"))
            ft_exit(line->pipe);
        else
    		return 1; // Command not recognized
        if (saved_output != -1)
        {
            if (dup2(saved_output, STDOUT_FILENO) == -1)
                perror("dup2");
            close(saved_output);
        }
        return (0); // Successfully executed the command
>>>>>>> a81b1ccc477598d27c263152b8076dd68edcd30a
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
 	if (parse_and_execute_solo_builtins(env, line->pipe) == 0)
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

int	pipex(t_env *env, t_line *line)
{
	if (!_call_childs(env, line))		
		return (0);
	return (1);
}
