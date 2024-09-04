/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:17:44 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/04 13:20:29 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *s1, const char *s2, size_t n)
{
	unsigned int	size;

	if (!s1 || !s2)
		return (s1);
	size = ft_strlen(s2);
	if (size >= n)
		ft_memcpy(s1, s2, n);
	else
	{
		ft_memcpy(s1, s2, size);
		ft_memset(s1 + size, '\0', n - size);
	}
	return (s1);
}
