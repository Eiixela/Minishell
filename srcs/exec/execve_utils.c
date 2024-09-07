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

int	check_command_exists(char **arg)
{
	if (!arg || !arg[0])
	{
		ft_putstr_fd("minishell: %s:command not found\n", 2);
		return (0);
	}
	return (1);
}

int	handle_builtin_execution(t_env *env, t_pipe *pipe, t_line *line)
{
	if (execute_builtins(env, pipe, line) == 1)
		return (1);
	return (0);
}

int	check_file_permissions(char *cmd)
{
	if (!access(cmd, F_OK) && access(cmd, F_OK | X_OK))
		return (0);
	return (1);
}

void	cleanup_resources(char **env_now, char *str, char *path)
{
	if (str)
		free(str);
	free_double_tab(env_now);
	if (path)
		free(path);
}

int	execute_external_command(t_env *env, t_pipe *pipe, char **env_now, \
	char *str)
{
	char	*path;

	path = get_path(pipe, env_now, -1);
	if (path == NULL)
	{
		cleanup_resources(env_now, str, NULL);
		error_and_free_for_exec(env, NULL, NULL, 1);
		return (0);
	}
	if (execve(path, pipe->arg, env_now) == -1)
	{
		cleanup_resources(env_now, str, path);
		error_and_free_for_exec(env, NULL, NULL, 2);
		return (0);
	}
	return (1);
}
