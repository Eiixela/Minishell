/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:12:31 by saperrie          #+#    #+#             */
/*   Updated: 2024/08/17 18:09:58 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// THERE'S GOTTA BE A LEAK IN THERE SOMEWHERE
char	*actual_expand(char *s1, char *value, char *rest)
{
	if (s1)
		return (ft_strjoin(s1, ft_strjoin(value, rest)));
	return (ft_strjoin(value, rest));
}

char	*get_env_value(t_line *line, char *name)
{
	char	**env;
	char	*value;
	int		name_len;

	env = line->env;
	name_len = ft_strlen(name);
	while (*env)
	{
		if (!ft_strncmp(name, *env, name_len) && *(*env + name_len) == '=')
		{
			if (*(*env + name_len + 1) == '\0')
				return (ft_calloc(1, 1));
			value = ft_strdup(*env + name_len + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		env++;
	}
	return (ft_calloc(1, 1));
}

char	*towards_expand(char *dollar_index, t_line *line, char *str_head)
{
	char	*final_input;
	char	*s1;
	char	*name;
	char	*value;
	char	*rest;
	char	*name_ptr;
	int		name_len;

	name_len = 0;
	final_input = ft_strdup(str_head);
	if (!final_input)
		return (false);
	s1 = ft_substr(str_head, 0, dollar_index - str_head);
	name_ptr = dollar_index;
	while (is_valid_varname(*(++name_ptr)))
		name_len += 1;
	name = ft_substr(dollar_index, 1, name_len);
	if (!name) // THERE SHOULD BE MORE TO THIS THAN JUST NULL CHECK
		return (NULL);
	value = get_env_value(line, name);
	if (!value)
		return (false);
	if (name)
		free(name);
	rest = _strdup(dollar_index + 1 + name_len);
	if (!rest)
		return (false);
	final_input = actual_expand(s1, value, rest);
	if (!final_input)
		return (false);
	// free_s1_value_rest(s1, value, rest);
	return (final_input);
}

char	*expand(char *input, t_line *line)
{
	char	*str_head;

	str_head = input;
	while (*input)
	{
		if (input[0] == '$' && (ft_isalpha(input[1]) || input[1] == '_'))
		{
			input = towards_expand(input, line, str_head);
			str_head = input;
			continue ;
		}
		if (!input)
			return (NULL);
		input += 1;
	}
	return (str_head);
}
