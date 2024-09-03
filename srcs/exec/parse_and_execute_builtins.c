/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   parse_and_execute_builtins.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/06/18 10:41:10 by aljulien  #+##+# */
/*   Updated: 2024/09/02 11:15:50 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtins(t_env *env, t_pipe *pipe, t_line *line)
{
	if (!ft_strcmp(pipe->arg[0], "echo"))
		line->pipe->ret_val = ft_echo(pipe->arg);
	else if (!ft_strcmp(pipe->arg[0], "cd"))
		line->pipe->ret_val = ft_cd(env, line);
	else if (!ft_strcmp(pipe->arg[0], "pwd"))
		line->pipe->ret_val = ft_pwd(pipe->arg);
	else if (!ft_strcmp(pipe->arg[0], "export"))
		line->pipe->ret_val = export(&pipe, env);
	else if (!ft_strcmp(pipe->arg[0], "unset"))
		line->pipe->ret_val = ft_unset(&line, env);
	else if (!ft_strcmp(pipe->arg[0], "env"))
		line->pipe->ret_val = ft_env(env, pipe);
	else if (!ft_strcmp(pipe->arg[0], "exit"))
		line->pipe->ret_val = ft_exit(pipe, line);
	else
		return (1);
	return (0);
}

int	parse_builtin(t_pipe *pipe)
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
int saved_output = -1;
if (line->pipe->next == NULL && parse_builtin(line->pipe) == 1 && line->pipe->arg)
{
if (line->pipe->redir != NULL)
if (!redirection_in_pipe(line->pipe, &saved_output, env))
return (0);
if (!execute_builtins(env, line->pipe, line))
{
if (saved_output != -1)
{
if (dup2(saved_output, STDOUT_FILENO) == -1)
perror("dup2");
close(saved_output);
}
return (2);  // Builtin was executed
}
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
