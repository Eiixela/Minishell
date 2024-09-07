/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   minishell.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: saperrie <saperrie@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/05/10 17:45:51 by aljulien  #+##+# */
/*   Updated: 2024/09/02 15:50:10 by saperrie ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

int	g_ret = 0;

static int init_env(t_env **env, char **envp)
{
	/*if (!isatty(0) || isatty(1))
		return (print_error(errno, "minishell"), 0);*/
	*env = NULL;
	create_env(envp, env);
	siglisten();
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char			*str;
	char			*cpy;
	static t_line	line;
	t_env			*env;
	int				status;

	(void)av;
	line.skipped_char = 0;
	str = NULL;
	status = 0;
	if (ac != 1)
		return (print_error(errno, "minishell: too many arguments"), 1);
	if (!init_env(&env, envp))
		return (1);
	line.env = env;
	while (1)
	{
		sigend();
		str = readline("aljulien@z3r8p5:~/goinfre/minishell$ ");
		if (!str)
			return (printf("exit\n"), cleanup(&line), free_env(env), 0);
		line.exit_status = status;
		if (str && *str && g_ret != SIGINT)
		{
			cpy = str;
			add_history(str);
			str = big_parse(&line, &str, status);
			if (str)
			{
				line.pipe->ret_val = status;
  			 	if (!pipex(env, &line, &status, str))
					perror("execve");
				line.exit_status = line.pipe->ret_val;
				if (str && str != cpy)
				{
					free(str - line.skipped_char);
					str = NULL;
				}
				cleanup(&line);
			}
			else
				cleanup(&line);
		}
		if (g_ret == SIGINT)
			status = 128 + g_ret;
		else
			status = line.exit_status;
	}
	free_env(env);
	clear_history();
	exit(status);
	return (0);
}
