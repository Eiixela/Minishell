/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   free.c :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: saperrie <saperrie@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/08/13 12:32:12 by aljulien  #+##+# */
/*   Updated: 2024/08/23 13:20:12 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

void	free_double_tab(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

void	free_all_tab(char **s_cmd, char **allpath)
{
	size_t	i;

	i = 0;
	while (s_cmd[i])
	{
		free(s_cmd[i]);
		i++;
	}
	free(s_cmd);
	i = 0;
	while (allpath[i])
	{
		free(allpath[i]);
		i++;
	}
	free(allpath);
}
