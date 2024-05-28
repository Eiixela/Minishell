/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:08:04 by aljulien          #+#    #+#             */
/*   Updated: 2024/05/28 11:38:07 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "inc/minishell.h"

# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((i < n) && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		write (fd, &s[i], 1);
		i++;
	}
}

static	void _cd_with_arg(char **av)
{
	int	r;

	if(av && av[1] && av[2])
	{
		r = chdir(av[2]);
		if (r == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror(av[2]);
		}
	}
}

void	ft_cd(char **av)
{
	char	*cwd;
	char	*key;
	char	*value;

	(void)av;
	value = getcwd(NULL, 0);
	_cd_with_arg(av);
	cwd = getcwd(NULL, 0);
	if (ft_strncmp(cwd, value, 100) != 0)
	{
		printf("wtf");
	}
	
}

int	main (int ac, char **av, char **env)
{
	char	*str;

	(void)av;
	(void)ac;
	(void)env;
	str = NULL;
	while (1)
	{
		str = readline("minishell >> ");
		if (str && *str)
		{
			ft_cd(av);
			add_history(str);
		}
	}
	rl_clear_history();
	return (0);
}