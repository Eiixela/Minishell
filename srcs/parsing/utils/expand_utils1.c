/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 01:10:21 by marvin            #+#    #+#             */
/*   Updated: 2024/08/29 01:10:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_env_var_format(char *input, short squote_mode)
{
	if (input[0] == '$' && (ft_isalpha(input[1]) || input[1] == '_') \
		&& squote_mode == -1 && input[1] != '$' \
			&& (input[1] != '\'' || input[1] != '"'))
		return (true);
	return (false);
}

bool	is_exit_status_format(char *input, short squote_mode)
{
	if (input[0] == '$' && input[1] == '?' && \
		(!is_valid_varname(input[2]) || !input[2]) && squote_mode == -1)
		return (true);
	return (false);
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
		return (false);
	free(name);
	return (value);
}
