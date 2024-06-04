/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:56:01 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/04 10:29:53 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <../libft/libft.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

enum e_TOKENS
{
	CMD,
	ARG,
	REDIR_OP,
	PIPE,
};

enum e_REDIR_OPERATOR
{
	ZERO,
	IN_REDIR,
	OUT_REDIR,
	APPEND,
	HEREDOC,
};

typedef struct s_redir
{
	// int				redir_index;
	char			type;
	char			*filename;
	// struct s_redir	*next;
	// struct s_redir	*prev;
}	t_redir;

typedef struct s_pipe
{
	char			**arg;
	t_redir			*redir;
	struct s_pipe	*next;
	struct s_pipe	*prev;
}	t_pipe;

typedef struct s_argv
{
	int				node_index;
	char			*node;
	struct s_argv	*next;
	struct s_argv	*prev;
}	t_argv;

typedef struct s_line
{
	int				argc;
	t_argv			*argv;
	t_argv			*argv_head;
	t_pipe			*pipe;
	char			**env;
}	t_line;



// =================================== PARSING ================================

int			main(int argc, char *argv[], char *exp[]);
bool		big_parse(t_line *line, char **input);
bool		lex(const char *input, t_line *line);
bool		parse(t_line *line);

// W_SPACE
bool		is_white_space(char c);
size_t	skip_white_spaces(const char **input);
// W_SPACE

// QUOTES
bool		quotes(const char *str);
const char		*find_matching_quote(const char *str, char quote);
bool		even_quotes(const char *str);
// QUOTES

// TOKENS_UTILS
bool		is_quote(char c, char quote);
const char	*skip_quote_content(const char *str, char quote);
// TOKENS_UTILS

// EXPANSION
bool		expand(t_line *line);
// EXPANSION

// STRUCT
t_line		*make_t_line_argv_node(const char *input, size_t len, t_line *line);
// STRUCT

// REDIRECTIONS
bool		good_redirections(const char *str);
const char		*bad_redirection(const char *str);
bool		is_valid_fd_name(char c);
char		is_redirection_operator(const char *str);
char		skip_redirection_operator(const char **str);
bool	clean_surrounding_quotes(t_line *line);

bool	process_redir(t_line *line, char redir_operator);
// REDIRECTIONS

// =================================== PARSING ================================

//====================================BUILTINS=================================

int		ft_echo(char **arg);
int		pwd(void);

//====================================BUILTINS=================================

// =================================== EXEC ================================

int		pipex(char **env, t_line line);
void	execute_cmd(char **env, char *cmd);
char	*get_path(char *cmd, char **env, int i);

// =================================== EXEC ================================


//====================================OTHERS===================================
void	ft_putstr_fd(char *s, int fd);



#endif