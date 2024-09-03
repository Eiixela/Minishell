/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   pwd.c :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/05/16 16:38:12 by aljulien  #+##+# */
/*   Updated: 2024/05/16 16:38:13 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_env *env)
{
	char	*str;

	str = NULL;
	str = find_var_env(env, "PWD=");
	if (!str || !str[0])
	{
		str = NULL;
		str = getcwd(str, 0);
		if (!str)
			return (print_error(errno, "minishell: exec"), 1);
		if (g_ret == SIGPIPE || ft_putendl_fd(str, STDOUT_FILENO) == -1)
			return (free(str), 1);
		free(str);
	}
	else if (g_ret == SIGPIPE || ft_putendl_fd(str, STDOUT_FILENO) == -1)
		return (free(str), 1);
	return (0);
}
	