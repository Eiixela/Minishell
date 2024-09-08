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

static void	free_for_no_access(t_env *env, t_line *line, char **env_now)
{
	print_error_message("minishell: ", line->pipe->arg[0], \
			" Permission denied\n");
	free_env(env);
	cleanup(line);
	free_double_tab(env_now);
	exit(126);
}

static void	free_for_no_path(t_env *env, t_line *line,
	char *path, char **env_now)
{
	free(path);
	free_env(env);
	free_double_tab(env_now);
	cleanup(line);
	exit(127);
}

static void	free_for_fail_execve(t_env *env, t_line *line,
	char *path, char **env_now)
{
	print_error_message("bash: ", line->pipe->arg[0], \
			" Is a directory\n");
	free_env(env);
	free_double_tab(env_now);
	free(path);
	cleanup(line);
	exit(126);
}

static void	free_str(char *str)
{
	if (str)
	{
		free(str);
		str = NULL;
	}
}

int	execute_cmd(t_env *env, t_pipe *pipe, t_line *line, char *str)
{
	char	*path;
	char	**env_now;

	env_now = NULL;
	path = NULL;
	if (!pipe->arg || !pipe->arg[0])
		return (ft_putstr_fd("minishell: command not found\n", 2), 0);
	if (execute_builtins(env, pipe, line) == 1)
	{
		env_now = arenvlst(env);
		if (!access(pipe->arg[0], F_OK))
			if (access(pipe->arg[0], F_OK | X_OK))
				free_for_no_access(env, line, env_now);
		path = get_path(pipe, env_now, -1);
		if (path == NULL)
		{
			print_error_message("minishell: ", pipe->arg[0],
				": command not found\n");
			free_str(str);
			free_for_no_path(env, line, path, env_now);
		}
		if (execve(path, pipe->arg, env_now) == -1)
			free_for_fail_execve(env, line, path, env_now);
	}
	return (1);
}
