/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   expand_utils2.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: marvin <marvin@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/08/29 01:13:31 by marvin#+##+# */
/*   Updated: 2024/08/29 01:13:31 by marvin   ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

void	free_s1_value_rest(char *s1, char *value, char *rest)
{
	if (s1)
		free(s1);
	if (value)
		free(value);
	if (rest)
		free(rest);
}

char	*handle_exit_status(char *input, t_line *line, char **str_head)
{
	char	*value;

	value = ft_itoa(line->exit_status);
	if (!value)
		return (NULL);
	input = towards_expand(input, line, *str_head, value);
	// free(*str_head);
	*str_head = input;
	return (input);
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
				return (ft_calloc(1, 1));
			value = ft_strdup(env->env + name_len + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		env = env->next;
	}
	return (ft_calloc(1, 1));
}

void	turn_extra_dollar_negative(char	**s1)
{
	char	*head;

	head = *s1;
	while (**s1)
	{
		if (**s1 == '$')
			**s1 *= -1;
		*s1 += 1;
	}
	*s1 = head;
}
