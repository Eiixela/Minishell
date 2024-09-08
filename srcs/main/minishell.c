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

static void	handle_exit(t_line *line, t_env *env, int status)
{
	printf("exit\n");
	cleanup(line);
	free_env(env);
	clear_history();
	exit(status);
}

static void	process_input(t_line *line, t_env *env, char *str, int *status)
{
	char	*cpy;

	cpy = str;
	add_history(str);
	str = big_parse(line, &str, *status);
	if (str)
	{
		if (str && str != cpy)
		{
			free(str - line->skipped_char);
			str = NULL;
		}
		line->pipe->ret_val = *status;
		if (!pipex(env, line, status, str))
			perror("execve");
		line->exit_status = line->pipe->ret_val;
		cleanup(line);
	}
	else
		cleanup(line);
}

static void	update_status(int *status, t_line *line)
{
	if (g_ret == SIGINT)
		*status = 128 + g_ret;
	else
		*status = line->exit_status;
}

int	main(int ac, char **av, char **envp)
{
	char	*str;
	t_line	line;
	t_env	*env;
	int		status;

	(void)av;
	initialize_variables(&line, &status);
	if (ac != 1)
		return (print_error(errno, "minishell: too many arguments"), 1);
	if (!initialize_environment(&env, envp))
		return (1);
	line.env = env;
	while (1)
	{
		sigend();
		str = readline("aljulien@z3r8p5:~/goinfre/minishell$ ");
		if (!str)
			handle_exit(&line, env, status);
		line.exit_status = status;
		if (str && *str && g_ret != SIGINT)
			process_input(&line, env, str, &status);
		update_status(&status, &line);
	}
	return (0);
}
