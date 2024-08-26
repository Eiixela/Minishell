/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:12:31 by saperrie          #+#    #+#             */
/*   Updated: 2024/08/23 09:23:26 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// THERE'S GOTTA BE A LEAK IN THERE SOMEWHERE
char	*actual_expand(char *s1, char *value, char *rest)
{
	char	*final_input;
	char	*s2;

	if (s1)
	{
		turn_extra_dollar_negative(&s1);
		s2 = ft_strjoin(value, rest);
		if (!s2)
			return (NULL);
		final_input = ft_strjoin(s1, s2);
		free(s2);
		return (final_input);
	}
	final_input = ft_strjoin(value, rest);
	return (final_input);
}

char	*get_env_value(char *name, t_env *env)
{
	t_env	*env_now;
	char	*value;
	int		name_len;

	env_now = env;
	name_len = ft_strlen(name);
	while (env_now)
	{
		if (!ft_strncmp(name, env_now->env, name_len) && *(env_now->env + name_len) == '=')
		{
			if (*(env_now->env + name_len + 1) == '\0')
				return (ft_calloc(1, 1));
			value = ft_strdup(env_now->env + name_len + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		env_now = env_now->next;
	}
	return (ft_calloc(1, 1));
}

void	free_s1_value_rest_name(char *s1, char *value, char *rest, char *name)
{
	if (s1)
		free(s1);
	if (value)
		free(value);
	if (rest)
		free(rest);
	if (name)
		free(name);
}

char	*towards_expand(char *dollar_index, char *str_head, t_env *env)
{
	char	*final_input;
	char	*s1;
	char	*name;
	char	*value;
	char	*rest;
	char	*name_ptr;
	int		name_len;

	name_len = 0;
	s1 = ft_substr(str_head, 0, dollar_index - str_head);
	name_ptr = dollar_index;
	while (is_valid_varname(*(++name_ptr)))
		name_len += 1;
	name = ft_substr(dollar_index, 1, name_len);
	if (!name) // THERE SHOULD BE MORE TO THIS THAN JUST NULL CHECK
		return (NULL);
	value = get_env_value(name, env);
	if (!value)
		return (false);
	rest = _strdup(dollar_index + 1 + name_len);
	if (!rest)
		return (false);
	final_input = actual_expand(s1, value, rest);
	if (!final_input)
		return (false);
	free_s1_value_rest_name(s1, value, rest, name);
	return (final_input);
}

char	*expand(char *input, t_env *env)
{
	char	*str_head;
	short	squote_mode;

	str_head = input;
	squote_mode = -1;
	while (*input)
	{
		if (input[0] == '\'')
			squote_mode *= -1;
		if (input[0] == '$' && (ft_isalpha(input[1]) || input[1] == '_') \
			&& squote_mode == -1 && input[1] != '$' && (input [1] != '\'' || input [1] != '"'))
		{
			input = towards_expand(input, str_head, env);
			str_head = input;
			continue ;
		}
		if (!input)
			return (NULL);
		input += 1;
	}
	return (str_head);
}
