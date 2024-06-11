/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:08:04 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/11 13:57:56 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((i < n) && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

static	char	*get_value_by_key(char *key, char **env)
{
	int	i;

	i = 0;
	while(env)
	{
		if (strncmp(env[i], key, 4))
			return (env[i]);
		i++;
	}
	return (NULL);
}

static	void _cd_with_arg(char **av)
{
	int	r;

	if(av && av[1] && av[2])
	{
		r = chdir(av[2]);
		if (r == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror(av[2]);
		}
	}
}

static void	_cd_without_arg(char **env)
{
	char	*err_str;
	char	*home;
	int		r;

	home = get_value_by_key("HOME", env);
	if (!home)
	{
		ft_putstr_fd("minishelll: cd: HOME not set\n", 2);
		return ;
	}
	r = chdir(home);
	if (r == -1)
	{
		err_str = ft_strjoin("minishell: cd:", home);
		if (!err_str)
			return ; //exit et free
		perror(err_str);
		free(err_str);
	}
}

void	ft_cd(char **av, char **env)
{
	char	*cwd;
	char	*key;
	char	*value;

	(void)av;
	value = getcwd(NULL, 0);
	if (av && av[0] && !av[1])
		_cd_without_arg(env);
	else
		_cd_with_arg(av);
	cwd = getcwd(NULL, 0);
	if (ft_strcmp(cwd, value) != 0)
	{
		key = ft_strdup("OLDPWD");
	}
	else
		fprintf(stderr, "free value in cd builtin");
	key = ft_strdup("PWD");
	value = getcwd(NULL, 0);
	
	
}