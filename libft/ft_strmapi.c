/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_strmapi.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/12 16:25:42 by alexie#+##+# */
/*   Updated: 2023/11/27 16:43:31 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*s2;
	unsigned int	len;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	s2 = malloc(len * sizeof(char) + 1);
	if (!s2)
		return (NULL);
	while (i < len)
	{
		s2[i] = (*f)(i, s[i]);
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
