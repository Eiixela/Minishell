/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:08:04 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/22 10:45:26 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr || !*arr)
		return (i);
	while (arr[i])
		i++;
	return (i);
}

static char	*init_path(t_line *line, t_env *env)
{
	char	*path;
	char	*temp;
	char	*str;

	path = NULL;
	temp = NULL;
	str = NULL;
	str = find_var_env(env, "PWD=");
	if (!str || !str[0])
	{
		str = NULL;
		str = getcwd(str, 0);
		if (!str)
			return (NULL);
		temp = ft_strjoin(str, "/");
		free(str);
	}
	else
		temp = ft_strjoin(str, "/");
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, line->pipe->arg[1]);
	free(temp);
	return (path);
}

static char	*cd_path(t_line *line, t_env *env)
{
	char	*path;
	t_env *env_now;

	env_now = env;
	if (line->pipe->arg[1][0] == '/')
		path = ft_strdup(line->pipe->arg[1]);
	else
		path = init_path(line, env_now);
	if (!path)
		return (print_error(errno, "minishell: exec"));
	path = prep_path(line->pipe->arg[1], path);
	if (!path)
		return (NULL);
	if (check_directory(line->pipe->arg[1], path))
		return (free(path), NULL);
	return (path);
}

static int	special_cases(t_line *line, char **path, t_env *env)
{
	t_env	*env_now;

	env_now = env;
	if (!line->pipe->arg[1])
	{
		*path = find_var_env(env_now, "HOME=");
		if (!*path || !*path[0])
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), -1);
		*path = ft_strdup(*path);
		if (!*path)
			return (print_error(errno, "minishell: exec"), -1);
		return (1);
	}
	else if (line->pipe->arg[1][0] == '-' && !line->pipe->arg[1][1])
	{
		*path = find_var_env(line->env, "OLDPWD=");
		if (!*path || !*path[0])
			return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2), -1);
		*path = ft_strdup(*path);
		if (!*path)
			return (print_error(errno, "minishell: exec"), -1);
		return (1);
	}
	return (0);
}

int	ft_cd(t_env *env, t_line *line)
{
	char	*path;
	char	*tmp;
	int		rv;

	path = NULL;
	if (ft_arrlen((line)->pipe->arg) > 2)
		return (print_error(0, "minishell: cd: too many arguments"), 1);
	rv = special_cases(line, &path, env);
	if (rv == -1)
		return (1);
	if (!rv)
	{
		path = cd_path(line, env);
		if (!path)
			return (1);
	}
	tmp = check_len(path, line->env);
	if (!tmp)
		return (free(path), 1);
	if (chdir(tmp) == -1)
		return (free(path), print_error(errno, "minishell: exec"), 1);
	if (pwds(env, path))
		return (free(path), print_error(errno, "minishell: exec"), 1);
	free(path);
	return (0);
}
