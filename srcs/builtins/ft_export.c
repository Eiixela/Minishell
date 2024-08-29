/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:08:05 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/29 10:37:14 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_sorted(char **tab)
{
	size_t	i;
	size_t	j;
	
	i = 0;
	j = ft_tablen(tab);
	if (tab)
	{
		while (i <= j - 1)
		{
			if (ft_strncmp(tab[i], tab[i + 1], 1) > 0)
				return (false);
			i++;
		}
	}
	return (true);
}

static char	**sort_tab(char **arenv, size_t len)
{
	size_t	i;
	size_t	j;
	char	*tenv;

	i = 0;
	while (i < 100 && arenv[i])
	{
		j = i + 1;
		if (j <= len && ft_strncmp(arenv[i], arenv[j], ft_strlen(arenv[i])) > 0)
		{
			tenv = arenv[i];
			arenv[i] = arenv[j];
			arenv[j] = tenv;
		}
		i++;
	}
	return (arenv);
}

static ssize_t	print_senv(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		if (printf("declare -x %s\n", arr[i]) == -1)
			return (1);
		i++;
	}
	return (1);
}

int	sort_env(t_env	*env)
{
	char	**arenv;
	size_t	len;
	ssize_t	rv;

	arenv = arenvlst(env);
	if (!arenv)
		return (verror("minishell: ", "export: ", strerror(errno)));
	len = ft_arrlen(arenv);
	while (!is_sorted(arenv))
		arenv = sort_tab(arenv, len);
	rv = print_senv(arenv);
	free_dtab(arenv);
	if (rv == -1)
		return (1);
	return (0);
}

static int check_arg(char *var)
{
    size_t i;

    i = 0;
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
    return (var[i] == '=' ? 1 : 0);
}


/* static int	change_var(t_env *env, char *var)
{
	char	*key;

	key = split_wsep(var, '=');
	if (!key)
		return (-1);
	while (env)
	{
		if (ft_strncmp(key, env->env, ft_strlen(key)) == 0)
		{
			free(env->env);
			free(key);
			env->env = ft_strdup(var);
			if (!env->env)
				return (-1);
			return (1);
		}
		env = env->next;
	}
	free(key);
	return (0);
} */

void *exprt_inenv_export(t_env **env, char *data, bool has_equals)
{
    t_env *new;

    new = env_newnode(data);
    if (!new)
        return (NULL);
    new->is_exported = !has_equals;  // Set to true if no '=', false if there's '='
    if (!has_equals)
    {
        // Handle case where there's no '=' sign
        free(new->env);
        new->env = ft_strdup(data);
        if (!new->env)
        {
            free(new);
            return (NULL);
        }
    }
    env_addback(env, new);
    return (new);
}

static int exec_export(t_pipe **pipe, t_env *head, t_env *env)
{
    size_t i;
    int rv;
    int status;

    status = 0;
    i = 0;
    while ((*pipe)->arg[++i])
    {
        rv = check_arg((*pipe)->arg[i]);
        if (rv >= 0)
        {
            t_env *current = head;
            char *var_name = (rv == 1) ? split_wsep((*pipe)->arg[i], '=') : ft_strdup((*pipe)->arg[i]);
            bool found = false;
            while (current)
            {
                if (ft_strncmp(var_name, current->env, ft_strlen(var_name)) == 0)
                {
                    if (rv == 1) // Variable with '='
                    {
                        free(current->env);
                        current->env = ft_strdup((*pipe)->arg[i]);
                        if (!current->env)
                            return (1);
                        current->is_exported = false;  // Set to 0 for variables with '='
                    }
                    else // Variable without '='
                    {
                        current->is_exported = true;  // Set to 1 for variables without '='
                    }
                    found = true;
                    break;
                }
                current = current->next;
            }
            if (!found) // If variable doesn't exist, create it
            {
                t_env *new_node = exprt_inenv_export(&env, (*pipe)->arg[i], rv == 1);
                if (!new_node)
                    return (1);
                new_node->is_exported = (rv == 0);  // Set to 1 if no '=', 0 if there's '='
            }
            free(var_name);
            if (!head)
                head = env;
        }
        else
            status = 1;
    }
    return (status);
}





int	export(t_pipe **pipe, t_env *env)
{
	t_env	*head;
	head = env;
	if (env && !(*pipe)->arg[1])
		if (sort_env(env) == 1)
			return (1);
	return (exec_export(pipe, head, env));
}