/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/28 09:00:09 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmd(t_env *env, t_pipe *pipe, t_line *line)
{
	char	*path;
	char	**env_now;

	env_now = arenvlst(env);
	path = NULL;
	if (!pipe->arg || !pipe->arg[0])
		return (ft_putstr_fd("minishell: %s:command not found\n", 2), 0);
	if (execute_builtins(env, pipe, line) == 1)
	{
		path = get_path(pipe, env_now, -1);
		if (path == NULL)
		{
			print_error_message("minishell: ", pipe->arg[0], \
		": command not found\n");
			free_double_tab(env_now);
			cleanup(line);
			free_env(env);
			exit(127);
		}
		if (execve(path, pipe->arg, env_now) == -1)
		{
			print_error_message("bash: ", pipe->arg[0], \
		" Is a directory");
			free_double_tab(env_now);
			cleanup(line);
			free_env(env);
			exit(126);
		}
	}
	free_double_tab(env_now);
	return (0);
}
