/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/05/31 17:45:29 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	parse_builtin(char **env, t_line line)
{
	(void)env;
	if (!ft_strcmp(&line.argv->av[0], "echo"))
		printf("echo\n");	//ft_echo()
	if (!ft_strcmp(&line.argv->av[0], "cd"))
		printf("cd\n");	//ft_cd()
	if (!ft_strcmp(&line.argv->av[0], "pwd"))
		printf("pwd\n");	//ft_pwd;
	if (!ft_strcmp(&line.argv->av[0], "export"))
		printf("export\n");	//ft_export()
	if (!ft_strcmp(&line.argv->av[0], "unset"))
		printf("unset\n");	//ft_unset()
	if (!ft_strcmp(&line.argv->av[0], "env"))
		printf("env\n");	//ft_env()
	if (!ft_strcmp(&line.argv->av[0], "exit"))
		printf("exit\n");	//ft_exit
	else 
		return (0);
	return (1);
}

static void	_child_action(char **env, t_line line, int pipefd[2])
{
	dup2(pipefd[1], STDERR_FILENO);
	if (!parse_builtin(env, line))
		execute_cmd(env, line.argv->av);
}

static int	first_child(char **env, int	pipefd[2], t_line line, size_t cmdnbr)
{
	pid_t	pid;

	cmdnbr = 0;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe "), 0);
	pid = fork();
	if (pid == -1)
	{
		return (perror("minishell: fork"), 0);
	}
	else if (pid == 0)
		_child_action(env, line, pipefd);
	close(pipefd[1]);
	close(pipefd[0]);
	return (1);
}

//TODO parent action function and executon from parent if command is a builtins
 static int	last_child(char **env, int pipefd[2], t_line line, size_t cmdnbr)
{
	pid_t	pid;

	pid = fork();
	if(pid == -1)
	{	
		if (cmdnbr != 0)
			close(pipefd[0]);
		return (perror("minishell: fork"), 0);
	}
	else if (pid == 0)
		_child_action(env, line, pipefd);
	// else if 
	//	_parent_action(env, line, pipefd);
	return (1);
}

static int	_call_childs(char **env, t_line line)
{
	size_t	cmdnbr;
	int		pipefd[2];

	cmdnbr = 0;
	if (cmdnbr == 1)
	{
		fprintf(stderr, "cc\n");
		if(!first_child(env, pipefd, line, cmdnbr))
			return (0);
		cmdnbr++;
	}
	last_child(env, pipefd, line, cmdnbr);
	return (1);
}

int	pipex(char **env, t_line line)
{
	if(!_call_childs(env, line))
		return (0);
	return (1);
}