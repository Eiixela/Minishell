/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:37:57 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/07 18:58:27 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_expanded_line(int fd, char *expanded_line)
{
	int	result;

	if (!expanded_line)
	{
		ft_putstr_fd("error: cannot expand variable\n", 2);
		return (0);
	}
	result = write_to_heredoc(fd, expanded_line);
	free(expanded_line);
	return (result);
}

static char	*get_env_value_heredoc(t_env *env, const char *var_name)
{
	t_env	*current;
	size_t	var_name_len;

	var_name_len = ft_strlen(var_name);
	current = env;
	while (current)
	{
		if (ft_strncmp(current->env, var_name, var_name_len) == 0
			&& current->env[var_name_len] == '=')
			return (current->env + var_name_len + 1);
		current = current->next;
	}
	return (NULL);
}

static int	process_variable(char **result, char *var_start,
	char *var_end, t_env *env)
{
	char	*name;
	char	*val;
	char	*new_result;
	size_t	new;

	while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
		var_end++;
	name = ft_strndup(var_start + 1, var_end - var_start - 1);
	if (!name)
		return (0);
	val = get_env_value_heredoc(env, name);
	if (val)
	{
		new = ft_strlen(*result) - ft_strlen(name) + ft_strlen(val);
		new_result = malloc(new + 1);
		if (!new_result)
			return (free(name), 0);
		ft_strlcpy(new_result, *result, var_start - *result + 1);
		ft_strlcat(new_result, val, new + 1);
		ft_strlcat(new_result, var_end, new + 1);
		free(*result);
		*result = new_result;
	}
	free(name);
	return (1);
}

char	*expand_variables(const char *input, t_env *env)
{
	char	*result;
	char	*var_start;
	char	*var_end;

	result = ft_strdup(input);
	if (!result)
		return (NULL);
	var_start = ft_strchr(result, '$');
	while (var_start != NULL)
	{
		var_end = var_start + 1;
		if (!process_variable(&result, var_start, var_end, env))
		{
			free(result);
			return (NULL);
		}
		var_start = ft_strchr(result, '$');
	}
	return (result);
}
