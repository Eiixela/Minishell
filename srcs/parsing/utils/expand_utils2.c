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

size_t	_strlen(char const *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (0);
	while (str[len])
		len++;
	return (len);
}

bool	is_valid_varname(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (true);
	return (false);
}

char	*_strdup(const char *s)
{
	char	*p;
	size_t	i;

	i = 0;
	p = (char *)malloc(sizeof(*s) * ft_strlen(s) + 1);
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
