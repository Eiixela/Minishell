/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   execve.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/05/28 14:14:35 by aljulien  #+##+# */
/*   Updated: 2024/09/02 16:23:48 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

static void	error_and_free_for_exec(t_env *env, t_line *line, char *path, int i)
{
	if (i == 0)
	{
		print_error_message("minishell: ", line->pipe->arg[0], \
			" Permission denied\n");;
		free_env(env);
		cleanup(line);
		exit(126);
	}
	if (i == 1)
	{
		print_error_message("minishell: ", line->pipe->arg[0], \
			": command not found\n");
		free(path);
		free_env(env);
		cleanup(line);
		exit(127);
	}
	if (i == 2)
	{
		print_error_message("bash: ", line->pipe->arg[0], \
			" Is a directory\n");
		cleanup(line);
		free(path);
		free_env(env);
		exit(126);
	}
}

int	execute_cmd(t_env *env, t_pipe *pipe, t_line *line, char *str)
{
	char	*path;
	char	**env_now;

	env_now = arenvlst(env);
	path = NULL;
	if (!pipe->arg || !pipe->arg[0])
		return (ft_putstr_fd("minishell: %s:command not found\n", 2), 0);
	if (execute_builtins(env, pipe, line) == 1)
	{
		if (!access(pipe->arg[0], F_OK))
		{
			if (access(pipe->arg[0], F_OK | X_OK))
			{
				free_double_tab(env_now);
				error_and_free_for_exec(env, line, path, 0);
			}
		}
		path = get_path(pipe, env_now, -1);
		if (path == NULL)
		{
			if (str)
			{
				free(str);
				str = NULL;
			}
			free_double_tab(env_now);
			error_and_free_for_exec(env, line, path, 1);
		}
		if (execve(path, pipe->arg, env_now) == -1)
		{
			free_double_tab(env_now);
			error_and_free_for_exec(env, line, path, 2);
		}
	}
	free_double_tab(env_now);
	return (1);
}
