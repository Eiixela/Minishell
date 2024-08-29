/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 09:31:24 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/29 15:29:00 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*find_var_env(t_env *env, char *var)
{
	while (env)
	{
		if (!ft_strncmp(env->env, var, ft_strlen(var)))
			return (env->env + ft_strlen(var));
		env = env->next;
	}
	return (NULL);
}

char	*split_wsep(char *str, char sep)
{
	size_t	i;
	char	*res;

	i = 0;
	if (!str || !sep)
		return (NULL);
	while (str[i] && str[i] != sep)
		i++;
	if (i == ft_strlen(str))
		return (NULL);
	res = ft_substr(str, 0, i + 1);
	if (!res)
		return (NULL);
	return (res);
}

void	*exprt_inenv(t_env **env, char *data)
{
	t_env	*new;

	new = env_newnode(data);
	if (!new)
		return (NULL);
	env_addback(env, new);
	return (new);
}

void free_for_export(int index, char **s, char **temp)
{
    if (*s)
    {
        free(*s);
        *s = NULL;
    }
    if (index == 0 && *temp)
    {
        free(*temp);
        *temp = NULL;
    }
    if (index == 2 && *temp)
    {
        free(*temp);
        *temp = NULL;
    }
}

int check_arg(char *var)
{
    size_t i;

    i = 0;
    if (var[0] == '-')
    {
		verror("minishell: export: '", var, "': invalid option");
        return (-2);
    }
    if (var[0] != '_' && !ft_isalpha(var[0]))
    {
        verror("minishell: export: '", var, "': not a valid identifier");
        return (-1);
    }
    while (var[i] && var[i] != '=')
    {
        if (var[i] != '_' && !ft_isalnum(var[i]))
        {
            verror("minishell: export: '", var, "': not a valid identifier");
            return (-1);
        }
        i++;
    }
    if (var[i] == '=')
        return (1);
    return (0);
}

