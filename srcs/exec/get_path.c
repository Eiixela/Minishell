/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:28:09 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/30 16:28:24 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*check_command_in_path(char *cmd)
{
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static void	free_all_tab(char **s_cmd, char **allpath)
{
	size_t	i;

	i = 0;
	while (s_cmd[i])
	{
		free(s_cmd[i]);
		i++;
	}
	free(s_cmd);
	i = 0;
	while (allpath[i])
	{
		free(allpath[i]);
		i++;
	}
	free(allpath);
}

static char	*get_env(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, 150) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

/* char	*get_path(char *cmd, char **env, int i)
{
	char	*exec;
	char	**s_cmd;
	char	*path_part;
	char	**allpath;

	if (ft_strchr(cmd, '/'))
		return (check_command_in_path(cmd));
	allpath = ft_split(get_env("PATH", env), ':');
	if (!allpath)
		return (NULL);
	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd)
		return (NULL);
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
			return (free_all_tab(s_cmd, allpath), exec);
		free(exec);
	}
	free_all_tab(s_cmd, allpath);
	return (NULL);
} */

char	*get_path(char **cmd, char **env, int i)
{
	char	*exec;
	char	*path_part;
	char	**allpath;

	if (ft_strchr(cmd[0], '/'))
		return (check_command_in_path(cmd[0]));
	allpath = ft_split(get_env("PATH", env), ':');
	if (!allpath)
		return (NULL);
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
			return (free_all_tab(cmd, allpath), exec);
		free(exec);
	}
	free_all_tab(cmd, allpath);
	return (NULL);
}
