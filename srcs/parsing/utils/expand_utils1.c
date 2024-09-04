/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   expand_utils1.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: marvin <marvin@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/08/29 01:10:21 by marvin#+##+# */
/*   Updated: 2024/08/29 01:10:21 by marvin   ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

// bool	is_env_var_format(char *input, short squote_mode)
// {
// 	if (input[0] == '$' && (ft_isalpha(input[1]) || input[1] == '_') \
// 		&& squote_mode == -1 && input[1] != '$')
// 		return (true);
// 	return (false);
// }

bool	is_env_var_format(char *input, short squote_mode)
{
	if (input[0] == '$' && (ft_isalpha(input[1]) || input[1] == '_') \
		&& squote_mode == -1 && input[1] != '$' \
			&& (input[1] == '\'' || input[1] == '"'))
		return (true);
	return (false);
}

bool	is_exit_status_format(char *input, short squote_mode)
{
	if (input[0] == '$' && input[1] == '?' && squote_mode == -1)
		return (true);
	return (false);
}

void	free_s1_value_rest(char *s1, char *value, char *rest)
{
	if (s1)
		free(s1);
	if (value)
		free(value);
	if (rest)
		free(rest);
}
