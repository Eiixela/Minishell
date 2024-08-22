/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute_builtins.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:41:10 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/22 10:32:53 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtins(t_env *env, t_pipe *pipe)
{
	if (!ft_strcmp(pipe->arg[0], "echo"))
		return(ft_echo(pipe->arg), 0);
 	if (!ft_strcmp(pipe->arg[0], "cd"))
		return(printf("cd"), 0); //ft_cd(pipe->arg, env)
	if (!ft_strcmp(pipe->arg[0], "pwd"))
		return (ft_pwd(pipe->arg), 0);
	if (!ft_strcmp(pipe->arg[0], "export"))
		return (printf("export\n"), 0); //ft_export()
	if (!ft_strcmp(pipe->arg[0], "unset"))
		return (printf("unset\n"), 0); //ft_unset()
	if (!ft_strcmp(pipe->arg[0], "env"))
		return (ft_env(env, pipe), 0);
	if (!ft_strcmp(pipe->arg[0], "exit"))
		return (ft_exit(pipe), 0);
	else
		return (1);
	return (0);
}

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
int parse_and_execute_solo_builtins(t_env *env, t_line *line)
{
    int saved_output;

    saved_output = -1;
    if (line->pipe->next == NULL && parse_builtin(line->pipe) == 1 && line->pipe->arg)
    {
        if (line->pipe->redir != NULL)
            if (!redirection_in_pipe(line->pipe, &saved_output))
                return (0);
        if (!ft_strcmp(line->pipe->arg[0], "echo"))
            ft_echo(line->pipe->arg);
        else if (!ft_strcmp(line->pipe->arg[0], "cd"))
            ft_cd(env, line);
        else if (!ft_strcmp(line->pipe->arg[0], "pwd"))
            ft_pwd(line->pipe->arg);
        else if (!ft_strcmp(line->pipe->arg[0], "export"))
            printf("export\n");
        else if (!ft_strcmp(line->pipe->arg[0], "unset"))
            printf("unset\n");
        else if (!ft_strcmp(line->pipe->arg[0], "env"))
            ft_env(env, line->pipe);
        else if (!ft_strcmp(line->pipe->arg[0], "exit"))
            ft_exit(line->pipe);
        else
            return (1);
        if (saved_output != -1)
        {
            if (dup2(saved_output, STDOUT_FILENO) == -1)
                perror("dup2");
            close(saved_output);
        }
        return (0);
    }
    return (1);
}

