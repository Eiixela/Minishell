/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 19:09:03 by saperrie          #+#    #+#             */
/*   Updated: 2024/08/22 09:06:52 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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