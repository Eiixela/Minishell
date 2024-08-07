/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/30 11:19:41 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		return (fprintf(stderr, "minishell: command not found : %s\n", pipe->arg[0]), 0);
	if (execute_builtins(env, pipe) == 1)
	{
		path = get_path(pipe->arg[0], env, -1);
		if (path == NULL)
		{
			fprintf(stderr, "minishell: %s: no such file or directory\n", pipe->arg[0]);
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
