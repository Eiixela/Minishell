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

void	error_and_free_for_exec(t_env *env, t_line *line, char *path, int i)
{
	free_env(env);
	if (i == 0)
	{
		print_error_message("minishell: ", line->pipe->arg[0], \
			" Permission denied\n");
		cleanup(line);
		exit(126);
	}
	if (i == 1)
	{
		print_error_message("minishell: ", line->pipe->arg[0], \
			": command not found\n");
		free(path);
		cleanup(line);
		exit(127);
	}
	if (i == 2)
	{
		print_error_message("bash: ", line->pipe->arg[0], \
			" Is a directory\n");
		cleanup(line);
		free(path);
		exit(126);
	}
}

int	execute_cmd(t_env *env, t_pipe *pipe, t_line *line, char *str)
{
	char	**env_now;

	if (!check_command_exists(pipe->arg))
		return (0);
	if (!handle_builtin_execution(env, pipe, line))
		return (1);
	env_now = arenvlst(env);
	if (!check_file_permissions(pipe->arg[0]))
	{
		cleanup_resources(env_now, NULL, NULL);
		error_and_free_for_exec(env, line, NULL, 0);
		return (0);
	}
	return (execute_external_command(env, pipe, env_now, str));
}
