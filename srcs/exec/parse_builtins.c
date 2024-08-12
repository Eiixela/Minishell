/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:41:10 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/12 17:20:20 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_builtin (t_pipe *pipe)
{
	if (pipe->arg)
	{
	if (!ft_strcmp(pipe->arg[0], "echo"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "cd"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "pwd"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "export"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "unset"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "env"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "exit"))
		return (1);
	}
	return (0);
}

//TODO make struct env for ome builtins
int parse_and_execute_solo_builtins(t_pipe *pipe)
{
    int saved_output;

    saved_output = -1;
    if (pipe->next == NULL && parse_builtin(pipe) == 1 && pipe->arg)
    {
        if (pipe->redir != NULL)
            if (!redirection_in_pipe(pipe, &saved_output))
                return (0);
        if (!ft_strcmp(pipe->arg[0], "echo"))
            ft_echo(pipe->arg); // Directly call the echo function
        else if (!ft_strcmp(pipe->arg[0], "cd"))
            ; // Implement cd logic
        else if (!ft_strcmp(pipe->arg[0], "pwd"))
            ft_pwd(pipe->arg);
        else if (!ft_strcmp(pipe->arg[0], "export"))
            printf("export\n");
        else if (!ft_strcmp(pipe->arg[0], "unset"))
            printf("unset\n");
        else if (!ft_strcmp(pipe->arg[0], "env"))
            ; // Implement env logic
        else if (!ft_strcmp(pipe->arg[0], "exit"))
            ft_exit(pipe);
        else
            return 1; // Command not recognized
        if (saved_output != -1)
        {
            if (dup2(saved_output, STDOUT_FILENO) == -1)
                perror("dup2");
            close(saved_output);
        }
        return (0); // Successfully executed the command
    }
    return (1); // No built-in command found
}

