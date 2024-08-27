/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:59:32 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/27 16:40:31 by aljulien         ###   ########.fr       */
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
# include <fcntl.h>
# include <limits.h>

# define EXIT_SHELL 42

extern int	g_ret;

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
	char			type;
	char			*fd;
	struct s_redir	*next;
	struct s_redir	*prev;
}	t_redir;

typedef struct s_env
{
	char			*env;
	struct s_env	*prev;
	bool 			is_exported;
	struct s_env	*next;
}	t_env;

typedef struct s_pipe
{
	char			**arg;
	t_redir			*redir;
	t_redir			*redir_head;
	struct s_pipe	*next;
	struct s_pipe	*prev;
	int				ret_val;
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
	t_pipe			*pipe_head;
	t_env			*env;
	int				nm_arg;
	t_env			*env_head;
}	t_line;


// =================================== PARSING ================================

int			main(int argc, char *argv[], char *exp[]);
bool		big_parse(t_line *line, char **input);
char		*expand(char *input, t_line *line);
bool		lex(char *input, t_line *line);
bool		parse(t_line *line);

// W_SPACE
bool		is_white_space(char c);
size_t		skip_white_spaces(char **input);

// QUOTES
char		*find_matching_quote(char *str, char quote);
bool		even_quotes(char *str);

// TOKENS_UTILS
bool		is_quote(char c, char quote);
char		*skip_quote_content(char *str, char quote);
bool		clean_surrounding_quotes(t_line *line);

// EXPAND_UTILS
size_t		_strlen(char const *str);
bool		is_valid_varname(char c);
char		*_strdup(const char *s);

// PARSING_UTILS
size_t		ft_tablen(char **str);
// bool		extract_node(t_line *line);

// STRUCT
t_line		*make_argv_node(char *input, size_t len, t_line *line);
size_t		count_argv_nodes(t_line *line);

// REDIRECTIONS
bool		handle_redir(t_line *line, char	*first_redirection);
bool		process_redir(t_line *line, char redir_operator, \
	char *first_redirection);
char		is_redirection_operator(char *str);
char		skip_redirection_operator(char **str);
char		redirection_offset(char redir_operator);

// CLEANUP
void		cleanup(t_line *line);
void	free_env(t_env *env);
// =================================== PARSING ================================

//======================================EXEC===================================

//BUILTINS
int		ft_echo(char **arg);
int		ft_pwd(char **av);
int		ft_cd(t_env *env, t_line *line);
int		ft_env(t_env *env, t_pipe *pipe);
int		ft_exit (t_pipe *pipe, t_line *line);
int		ft_unset(t_line **line, t_env *env);
int		parse_builtin(t_pipe *pipe);
char	**arenvlst(t_env	*env);
int		check_directory(char *var, char *path);
int		pwds(t_env *env, char *path);
char	*check_len(char	*path, t_env *env);
char	*prep_path(char *var, char *path);
char	*find_var_env(t_env *env, char *var);
size_t	ft_arrlen(char **arr);
char	*split_wsep(char *str, char sep);
void	*exprt_inenv(t_env **env, char *data);
int		export(t_pipe **pipe, t_env *env);
int		sort_env(t_env	*env);

//ERROR
void	*errjoin(int error_code, char *error_message);
void	*print_error(int error_code, char *error_message);
int		print_error_message(char *s1, char *s2, char *s3);
int		verror(char *s1, char *s2, char *s3);

//FREE
void		env_freelst(t_env **env);
void		env_addback(t_env **env, t_env *node);
t_env		*env_newnode(char *data);
void		free_all_tab(char **s_cmd, char **allpath);
void		free_double_tab(char **s);

//SIGNALS
char		*send_eof(char *line);
void		siglisten(void);
void		sigend(void);
void		sighandler(int sig);
void handle_exit_status_child(t_line *line, int status, int *quit_message_printed);

//REDIRECTIONS
int			redirection_in_pipe(t_pipe *pipe, int *saved_output);

//EXECUTING
int		pipex(t_env *env, t_line *line, int *status);
int		execute_cmd(t_env *env, t_pipe *pipe, t_line *line);
char	*get_path(t_pipe *pipe, char **env, int i);
int		parse_and_execute_solo_builtins(t_env *env, t_line *line);
int		create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd, t_line *line);
void	create_env(char **envp, t_env **env);
int		_call_childs(t_env *env, t_line *line);
int		execute_builtins(t_env *env, t_pipe *pipe, t_line *line);

// =================================== EXEC ================================

#endif
