/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:12:31 by saperrie          #+#    #+#             */
/*   Updated: 2024/08/14 17:22:56 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*_strdup(const char *s)
{
	char	*p;
	size_t	i;

	i = 0;
	p = (char *)malloc(sizeof(*s) * _strlen(s) + 1);
	if (p == NULL)
		return (NULL);
	while (s[i])
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

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
			value = ft_strdup(*env + name_len + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		env++;
	}
	return (ft_calloc(1, 1));
}

char	*get_value(char	*dollar_index, t_line *line)
{
	char	*ptr;
	char	*name;
	char	*value;
	size_t	name_len;

	name_len = 0;
	ptr = dollar_index;
	while (is_valid_varname(*(++ptr)))
		name_len += 1;
	name = ft_substr(dollar_index, 1, name_len);
	if (!name) // THERE SHOULD BE MORE TO THIS THAN JUST NULL CHECK
		return (NULL);
	value = get_env_value(line, name);
	return (value);
}

char	*towards_expand(char *dollar_index, t_line *line, char *str_head)
{
	char	*final_input;
	char	*s1;
	char	*value;
	char	*rest;

	final_input = ft_strdup(str_head);
	if (!final_input)
		return (false);
	s1 = ft_substr(str_head, 0, dollar_index - str_head);
	value = get_value(dollar_index, line);
	if (!value)
		return (false);
	rest = _strdup(dollar_index + 1 + _strlen(value));
	if (!rest)
		return (false);
	final_input = actual_expand(s1, value, rest);
	if (!final_input)
		return (false);
	// free_s1_value_rest(s1, value, rest);
	// printf("INPUTPUT: %s\n", final_input);
	// pause();
	return (final_input);
}

char	*expand(char *input, t_line *line)
{
	char	*str_head;

	str_head = input;
	while (*input)
	{
		if (input[0] == '$' && (input[1] != '+' || input[1] != '=')) /* + = / . , : ~ ` ! # $ % ^ ( ) { } [ ] \ PRINT DOLLAR*/ // FOR OTHERS SPECIAL CHARACTERS DON'T (AND )
		{
			input = towards_expand(input, line, str_head);
			str_head = input;
			continue ;
		}	
		// else if (*input == '$' && (input[1] == '+' || input[1] == '=')) /* + = / . , : ~ ` ! # $ % ^ ( ) { } [ ] \ PRINT DOLLAR*/ // FOR OTHERS SPECIAL CHARACTERS DON'T (AND )
		// 	PRINT_DOLLAR_AND_FRIENDS();
		if (!input)
			return (NULL);
		input += 1;
	}
	return (str_head);
}
