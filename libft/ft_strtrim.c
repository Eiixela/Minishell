/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:23:12 by alexie            #+#    #+#             */
/*   Updated: 2023/11/27 16:35:24 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_charset(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*s;

	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_charset(s1[start], set))
		start++;
	while (end > start && ft_charset(s1[end - 1], set))
		end--;
	s = (char *)malloc(sizeof(*s1) * (end - start + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (start < end)
	{
		s[i] = s1[start];
		i++;
		start++;
	}
	s[i] = '\0';
	return (s);
}
