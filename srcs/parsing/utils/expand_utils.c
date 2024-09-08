/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   expand_utils.c :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/07/17 19:09:03 by saperrie  #+##+# */
/*   Updated: 2024/08/17 17:57:03 by saperrie ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

void	turn_value_negative(char *value)
{
	if (!*value)
		*value = -1;
	while (*value)
	{
		if (*value == '|' || is_quote(*value, '\'') || is_quote(*value, '\"'))
			*value *= -1;
		value += 1;
	}
}

char	*get_value(char *dollar_index, t_line *line, char	*name, \
	int *name_len)
{
	char	*value;
	char	*name_ptr;

	name_ptr = dollar_index;
	while (is_valid_varname(*(++name_ptr)))
		*name_len += 1;
	name = ft_substr(dollar_index, 1, *name_len);
	if (!name)
		return (NULL);
	value = get_env_value(line, name);
	if (!value)
		return (NULL);
	free(name);
	turn_value_negative(value);
	return (value);
}

char	*get_env_value(t_line *line, char *name)
{
	t_env	*env;
	char	*value;
	int		name_len;

	env = line->env;
	name_len = ft_strlen(name);
	while (env)
	{
		if (!ft_strncmp(name, env->env, name_len) \
			&& *(env->env + name_len) == '=')
		{
			if (*(env->env + name_len + 1) == '\0')
				return (ft_calloc(2, 2));
			value = ft_strdup(env->env + name_len + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		env = env->next;
	}
	return (ft_calloc(2, 2));
}
