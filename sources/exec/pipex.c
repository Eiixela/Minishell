/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/07 17:22:04 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	parse_builtin(char **env, t_line line)
{
	(void)env;
	if (!ft_strcmp(line.pipe->arg[0], "echo"))
		printf("echo\n");	//ft_echo()
	if (!ft_strcmp(line.pipe->arg[0], "cd"))
		printf("cd\n");	//ft_cd()
	if (!ft_strcmp(line.pipe->arg[0], "pwd"))
		printf("pwd\n");	//ft_pwd;
	if (!ft_strcmp(line.pipe->arg[0], "export"))
		printf("export\n");	//ft_export()
	if (!ft_strcmp(line.pipe->arg[0], "unset"))
		printf("unset\n");	//ft_unset()
	if (!ft_strcmp(line.pipe->arg[0], "env"))
		printf("env\n");	//ft_env()
	if (!ft_strcmp(line.pipe->arg[0], "exit"))
		printf("exit\n");	//ft_exit
	else 
		return (0);
	return (1);
}

static int	open_file(t_line line, int in_or_out)
{
	int	fd;

	if (in_or_out == 1)
	{
		fd = open(line.pipe->redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0777); //644
		if (fd == -1)
		{
			ft_putstr_fd("cannot open or create outfile", 2);
			return (0);		
		}
	}
	if (in_or_out == 2)
	{
		fd = open(line.pipe->redir->filename, O_RDONLY, 0777);
		if (fd == -1)
		{
			ft_putstr_fd("cannot open or create outfile", 2);
			return (0);		
		}
	}
	return (fd);
}

static void	_child_action(char **env, t_line line, int pipefd[2], int cmdnbr)
{
	int	fd_out = 0;
	int	fd_in;
	
	if (line.pipe->redir->type == OUT_REDIR)
	{
		fprintf(stderr, "cc1");
		fd_out = open_file(line, 1);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			//close and free 
			fprintf(stderr, "ca marche pas frr dans le dup2 du outredir child_action\n\n\n");
			return ;
		}
		//close(fd_out);
	}
	if (line.pipe->redir->filename && line.pipe->redir->type == IN_REDIR)
	{
		fprintf(stderr, "cc2");
		fd_in = open_file(line, 2);
		dup2(fd_in, STDIN_FILENO);
		if (cmdnbr != 0)
			close (pipefd[0]);
	}
 	else if (cmdnbr != 0)
		dup2 (pipefd[0], STDIN_FILENO); 	
	if (!parse_builtin(env, line))
		execute_cmd(env, &line.pipe->arg[0]);
 	if (cmdnbr != 0)
		close (pipefd[0]);
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
		_child_action(env, line, pipefd, cmdnbr);
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
		_child_action(env, line, pipefd, cmdnbr);
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