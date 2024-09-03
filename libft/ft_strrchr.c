/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_strrchr.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/07 15:12:42 by aljulien  #+##+# */
/*   Updated: 2023/11/27 16:43:36 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	len;

	len = ft_strlen(s);
	i = 0;
	while (s[i])
		i++;
	while (i >= 0 && i < len + 1)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i--;
	}
	return (NULL);
}
