/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:27:47 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/08 17:27:49 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_exit_status(char *input, t_line *line, char **str_head)
{
	char	*value;

	value = ft_itoa(line->exit_status);
	if (!value)
		return (NULL);
	input = towards_expand(input, line, *str_head, value);
	free(*str_head - line->skipped_char);
	line->skipped_char = 0;
	*str_head = input;
	return (input);
}

char	*actual_expand(char **s1, char *value, char *rest)
{
	char	*final_input;
	char	*s2;

	if (s1 && *s1 && **s1)
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
	s1 = NULL;
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
	free_s1_value_rest(s1, value, rest);
	return (final_input);
}

char	*expand(char *input, t_line *line, char *value, short squote_mode)
{
	char		*str_head;
	short		dquote_mode;

	str_head = input;
	dquote_mode = -1;
	while (input && *input)
	{
		value = NULL;
		quote_mode(input, &squote_mode, &dquote_mode);
		if (is_env_var_format(input, squote_mode))
		{
			input = handle_env_var(input, line, &str_head, value);
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
