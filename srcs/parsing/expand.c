/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   expand.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: marvin <marvin@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/08/14 12:12:31 by saperrie  #+##+# */
/*   Updated: 2024/08/29 01:21:29 by marvin   ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

char	*handle_exit_status(char *input, t_line *line, char **str_head)
{
	char	*value;

	value = ft_itoa(line->exit_status);
	if (!value)
		return (NULL);
	input = towards_expand(input, line, *str_head, value);
	free(*str_head);
	*str_head = input;
	return (input);
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

char	*towards_expand(char *dollar_index, t_line *line, char *str_head, \
	char *value)
{
	char	*final_input;
	char	*s1;
	char	*name;
	char	*rest;
	int		name_len;

	name = NULL;
	name_len = 0;
	s1 = ft_substr(str_head, 0, dollar_index - str_head);
	if (!value)
	{
		value = get_value(dollar_index, line, name, &name_len);
		if (!value)
			return (false);
	}
	else
		name_len = 1;
	rest = _strdup(dollar_index + 1 + name_len);
	if (!rest)
		return (false);
	final_input = actual_expand(&s1, value, rest);
	if (!final_input)
		return (false);
	free_s1_value_rest(s1, value, rest);
	return (final_input);
}

char	*expand(char *input, t_line *line, char *value, short squote_mode)
{
	char		*str_head;

	str_head = input;
	while (*input)
	{
		value = NULL;
		if (input[0] == '\'')
			squote_mode *= -1;
		if (is_env_var_format(input, squote_mode))
		{

			input = towards_expand(input, line, str_head, value);
			free(str_head);
			str_head = input;
			continue ;
		}
		if (is_exit_status_format(input, squote_mode))
		{
			input = handle_exit_status(input, line, &str_head);
			continue ;
		}
		if (!input)
			return (NULL);
		input += 1;
	}
	return (str_head);
}
