/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_putendl_fd.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/11 22:41:08 by alexie#+##+# */
/*   Updated: 2024/08/13 10:40:22 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

ssize_t	ft_putendl_fd(char *s, int fd)
{
	ssize_t	len;

	len = write(fd, s, ft_strlen(s));
	if (len == -1)
		return (len);
	len += write(fd, "\n", 1);
	if (len == -1)
		return (len);
	return (len);
}
