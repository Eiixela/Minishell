/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:08:04 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/12 09:07:04 by aljulien         ###   ########.fr       */
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

static char *get_value_by_key(const char *key, char **env)
{
    int i = 0;
    size_t key_len = strlen(key);

    while (env && env[i])
    {
        if (strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
            return env[i] + key_len + 1;
        i++;
    }
    return NULL;
}

void update_pwd(char **env)
{
    char *oldpwd = get_value_by_key("PWD", env);
    char *new_pwd = getcwd(NULL, 0);

    if (oldpwd)
    {
        // Here you would set the environment variable OLDPWD
        setenv("OLDPWD", oldpwd, 1);
    }
    // Set the environment variable PWD
    setenv("PWD", new_pwd, 1);
    free(new_pwd);
}

static void _cd_with_arg(char **av, char **env)
{
    if (av && av[1])
    {
        if (chdir(av[1]) == -1)
        {
            perror("minishell: cd");
        }
        else
        {
            update_pwd(env);
        }
    }
}

static void _cd_without_arg(char **env)
{
    char *home = get_value_by_key("HOME", env);
    if (!home)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", 2);
        return;
    }
    if (chdir(home) == -1)
    {
        perror("minishell: cd");
    }
    else
    {
        update_pwd(env);
    }
}


void	ft_cd(char **av, char **env)
{
	char	*cwd;
	char	*key;
	char	*value;

    (void)key;
	(void)av;
	value = getcwd(NULL, 0);
	if (av && av[0] && !av[1])
		_cd_without_arg(env);
	else
		_cd_with_arg(av, env);
	cwd = getcwd(NULL, 0);
	if (ft_strncmp(cwd, value, 100) != 0)
		key = ft_strdup("OLDPWD");
	else
		fprintf(stderr, "free value in cd builtin\n");
	key = ft_strdup("PWD");
	value = getcwd(NULL, 0);
	
	
}