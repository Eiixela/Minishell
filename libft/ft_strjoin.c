/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:26:20 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/08 17:26:22 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*s;
	unsigned int	len_s1;
	unsigned int	len_s2;
	size_t			i;
	size_t			j;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	s = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!s)
		return (NULL);
	i = 0;
	while (i < len_s1)
	{
		s[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len_s2)
		s[i++] = s2[j++];
	s[i] = '\0';
	return (s);
}
