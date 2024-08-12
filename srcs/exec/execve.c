/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/12 16:56:43 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtins(char **env, t_pipe *pipe)
{
	if (!ft_strcmp(pipe->arg[0], "echo"))
		return(ft_echo(pipe->arg), 0);
 	if (!ft_strcmp(pipe->arg[0], "cd"))
		return(ft_cd(pipe->arg, env), 0);
	if (!ft_strcmp(pipe->arg[0], "pwd"))
		return (ft_pwd(pipe->arg), 0);
	if (!ft_strcmp(pipe->arg[0], "export"))
		return (printf("export\n"), 0); //ft_export()
	if (!ft_strcmp(pipe->arg[0], "unset"))
		return (printf("unset\n"), 0); //ft_unset()
	if (!ft_strcmp(pipe->arg[0], "env"))
		return (ft_env(env), 0);
	if (!ft_strcmp(pipe->arg[0], "exit"))
		return (ft_exit(pipe), 0);
	else
		return (1);
	return (0);
}

int	execute_cmd(char **env, t_pipe *pipe)
{
	char	*path;
	path = NULL;
	if (!pipe->arg || !pipe->arg[0])
		return (printf("minishell: %s:command not found\n", pipe->arg[0]), 0);
	if (execute_builtins(env, pipe) == 1)
	{
		path = get_path(pipe, env, -1);
		if (path == NULL)
		{
			ft_putstr_fd("minishell: :command not found\n", STDOUT_FILENO);
			exit(127);
		}
		if (execve(path, pipe->arg, env) == -1)
		{
			perror("execve");
			exit(126);
		}
	}
	return (0);
}
