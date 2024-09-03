/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_lstnew_bonus.c  :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/22 15:07:49 by aljulien  #+##+# */
/*   Updated: 2023/11/27 16:42:43 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
