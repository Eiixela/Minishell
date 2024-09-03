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

#include "../../inc/minishell.h"

int	ft_pwd(char **av)
{
	char	*pwd;

	if (av && av[0] && av[1] && (ft_strlen(av[1]) >= 2) && (ft_strncmp(av[1], "-", 1) == 0))
	{
		av[1][2] = '\0';
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putstr_fd(av[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		perror("minishell: pwd");
	else
	{
		ft_putstr_fd(pwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		free(pwd);
	}
	return (0);
}