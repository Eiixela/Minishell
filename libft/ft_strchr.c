/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_schr.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42lyon.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/07 15:01:52 by aljulien  #+##+# */
/*   Updated: 2023/11/18 14:40:36 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ptr;

	if ((char) c == '\0')
		return ((char *)&s[ft_strlen(s)]);
	ptr = (char *)s;
	while (*ptr)
	{
		if (*ptr == (char)c)
			return (ptr);
		ptr++;
	}
	return (NULL);
}
