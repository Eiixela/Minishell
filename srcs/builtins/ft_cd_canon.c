/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_canon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:28:16 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/08 17:28:18 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_path(char *tab, char *path, char **temp)
{
	char	*tmp;

	tmp = NULL;
	free(*temp);
	*temp = NULL;
	*temp = ft_strdup(path);
	if (!*temp)
		return (free(path), print_error(errno, "minishell: exec"));
	tmp = ft_strjoin(path, tab);
	free(path);
	if (!tmp)
		return (free(*temp), print_error(errno, "minishell: exec"));
	path = tmp;
	tmp = ft_strjoin(path, "/");
	free(path);
	if (!tmp)
		return (free(*temp), print_error(errno, "minishell: exec"));
	path = tmp;
	return (path);
}

static int	rm_dots(char **path, char **temp, char *tab, char *var)
{
	size_t	len;

	if (check_directory(var, *path))
		return (free(*temp), free(*path), 1);
	free(*path);
	*path = NULL;
	*path = ft_strdup(*temp);
	if (!*path)
		return (free(*temp), 1);
	len = ft_strlen(tab) + 1;
	free(*temp);
	*temp = ft_substr(*path, 0, ft_strlen(*path) - len);
	if (!*temp)
		return (free(*path), 1);
	return (0);
}

static char	*canonical_form(char *var, char *path, char **tab, ssize_t j)
{
	ssize_t	i;
	char	*temp;

	i = -1;
	temp = NULL;
	while (tab[++i])
	{
		if (!ft_strncmp(tab[i], "..", 3) && (i - j) >= 0
			&& ft_strncmp(tab[i - j], ".", 2)
			&& ft_strncmp(tab[i - j], "..", 3))
		{
			if ((i - (j + 1)) >= 0)
				j++;
			if (rm_dots(&path, &temp, tab[i - j], var))
				return (NULL);
			j++;
		}
		else if (ft_strncmp(tab[i], ".", 2))
		{
			path = make_path(tab[i], path, &temp);
			if (!path)
				return (NULL);
		}
	}
	return (free(temp), path);
}

static int	just_slash(char **path)
{
	int		i;
	char	*temp;

	i = 0;
	temp = *path;
	while (temp[i] && temp[i] == '/')
		i++;
	if (temp[i] == '\0')
	{
		free(*path);
		*path = NULL;
		*path = ft_strdup("/");
		if (!*path)
			print_error(errno, "minishell: exec");
		return (1);
	}
	return (0);
}

char	*prep_path(char *var, char *path)
{
	char	**tab;
	char	*temp;

	temp = NULL;
	if (just_slash(&path))
		return (path);
	tab = ft_split(path, '/');
	free(path);
	path = NULL;
	if (!tab)
		return (print_error(errno, "minishell: exec"));
	temp = ft_strdup("/");
	if (!temp)
		return (free_dtab(tab), print_error(errno, "minishell: exec"));
	path = canonical_form(var, temp, tab, 1);
	free_dtab(tab);
	if (!path && errno != 2)
		return (print_error(errno, "minishell: exec"));
	return (path);
}
