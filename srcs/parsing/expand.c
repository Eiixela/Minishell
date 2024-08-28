/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:12:31 by saperrie          #+#    #+#             */
/*   Updated: 2024/08/27 20:41:46 by saperrie         ###   ########.fr       */
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

char	*actual_expand(char **s1, char *value, char *rest)
{
	char	*final_input;
	char	*s2;

	if (s1 && *s1)
	{
		turn_extra_dollar_negative(s1);
		s2 = ft_strjoin(value, rest);
		if (!s2)
			return (NULL);
		final_input = ft_strjoin(*s1, s2);
		free(s2);
		s2 = NULL;
		return (final_input);
	}
	final_input = ft_strjoin(value, rest);
	return (final_input);
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

char	*towards_expand(char *dollar_index, t_line *line, char *str_head, char* value)
{
	char	*final_input;
	char	*s1;
	char	*name;
	char	*rest;
	char	*name_ptr;
	int		name_len;

	name_len = 0;
	s1 = ft_substr(str_head, 0, dollar_index - str_head);
	name_ptr = dollar_index;
	// init_value();
	if (!value)
	{
		while (is_valid_varname(*(++name_ptr)))
			name_len += 1;
		name = ft_substr(dollar_index, 1, name_len);
		if (!name)
			return (NULL);
		value = get_env_value(line, name);
		if (!value)
			return (false);
	}
	else
	{
		name = NULL;
		name_len = 1;
	}
	rest = _strdup(dollar_index + 1 + name_len);
	if (!rest)
		return (false);
	final_input = actual_expand(&s1, value, rest);
	if (!final_input)
		return (false);
	free_s1_value_rest_name(s1, value, rest, name);
	return (final_input);
}

char	*expand(char *input, t_line *line)
{
	char	*str_head;
	short	squote_mode;
	char	*value;

	str_head = input;
	squote_mode = -1;
	while (*input)
	{
		value = NULL;
		if (input[0] == '\'')
			squote_mode *= -1;
		if (input[0] == '$' && (ft_isalpha(input[1]) || input[1] == '_') \
			&& squote_mode == -1 && input[1] != '$' \
				&& (input[1] != '\'' || input[1] != '"'))
		{
			input = towards_expand(input, line, str_head, value);
			free(str_head);
			str_head = input;
			continue ;
		}
		else if (input[0] == '$' && input[1] == '?' && \
			(!is_valid_varname(input[2]) || !input[2]) && squote_mode == -1)
		{
			value = ft_itoa(line->exit_status);
			if (!value)
				return (NULL);
			input = towards_expand(input, line, str_head, value);
			free(str_head);
			str_head = input;
			continue ;
		}
		if (!input)
			return (NULL);
		input += 1;
	}
	return (str_head);
}
