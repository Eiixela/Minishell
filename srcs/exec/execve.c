/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/13 15:02:12 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmd(t_env *env, t_pipe *pipe)
{
	char	*path;
	char	**env_now;

	env_now = arenvlst(env);
	path = NULL;
	if (!pipe->arg || !pipe->arg[0])
		return (printf("minishell: %s:command not found\n", pipe->arg[0]), 0);
	if (execute_builtins(env, pipe) == 1)
	{
		path = get_path(pipe, env_now, -1);
		if (path == NULL)
		{
			ft_putstr_fd("minishell: ", STDOUT_FILENO);
			ft_putstr_fd(pipe->arg[0], STDOUT_FILENO);
			ft_putstr_fd(": command not found\n", STDOUT_FILENO);
			exit(127);
		}
		if (execve(path, pipe->arg, env_now) == -1)
		{
			perror("execve");
			exit(126);
		}
	}
	return (0);
}
