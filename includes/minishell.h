#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 30

#  include "../libft/libft.h"
#  include <stdbool.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <string.h>
#  include <sys/stat.h>
#  include <signal.h>
#  include <sys/wait.h>

#  define SUCCESS 0

int	g_exit_status;

enum e_pipe_fd
{
	READ,
	WRITE
};

enum e_env
{
	ENV,
	EXPORT
};

enum e_process
{
	PROCESS,
	CHILD
};

enum e_status
{
	OFF,
	ON
};

enum e_redir_type
{
	IN = 1,
	OUT,
	DBL_IN,
	DBL_OUT,
	IN_OUT
};

enum e_error_type
{
	ERROR = -1,
	FATAL_ERROR = -2,
	SYNTAX_ERROR = -3,
	OPEN_ERROR = -4,
	CHDIR_ERROR = -5,
	STAT_ERROR = -6,
	HOME_ERROR = -7,
	GETCWD_ERROR = -8,
	WAITPID_ERROR = -9,
	CLOSE_ERROR = -10,
	DUP_ERROR = -11,
	DUP2_ERROR = -12,
	PIPE_ERROR = -13,
	FORK_ERROR = -14,
	SIGNAL_ERROR = -15
};

typedef struct s_env
{
	t_list	*vars;
	t_list	*exp;
	char	**paths;
	char	**envp;
	int		flag;
	int		is_pipe;
	int		read_fd;
	int		saved_stdin;
	int		saved_stdout;
	char	*saved_pwd;
}			t_env;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	char			**argv;
	char			operator;
	int				fd_in;
	int				fd_out;
	int				saved_fd_in;
	int				saved_fd_out;
	int				pipe_fd[2];
	pid_t			pid;
	struct s_rd		*rd_head;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_quote
{
	int			dq;
	int			sq;
}				t_quote;

typedef struct s_rd
{
	int				type;
	char			*fn;
	struct s_rd		*next;
}					t_rd;

typedef struct s_vars
{
	t_quote	state;
	int		i;
	int		j;
	int		k;
	size_t	len;
	int		start;
	int		count;
	int		pipe;
	int		flag;
	int		ret;
}			t_vars;

/* main.c */
void	prompt(void);

/* check_input.c */
bool	is_whitespace(char c);
bool	empty_line(char *line);
bool	empty_pipe(char *str, int pipe_count);
bool	is_separator(t_quote *state);
bool	quote_state(t_quote *state, char c);

/* get_next_line_eof.c */
int		get_next_line_eof(int fd, char **line);

/* io_stream.c */
int		save_iostream(t_env *env);
int		close_saved_fds(t_env *env);
void	change_pipe_streams(t_cmd *head, t_env *env);
int		check_o_stream(t_env *env);

/* parser.c */
int		parser(t_env *env, t_cmd **head, t_vars *var, char *line);

/* process_input.c */
int		process_input(t_env *env, char *input);

/* signals.c */
int		catch_signals(t_env *env);
int		reset_sig(t_env *env);
void	signal_handler(int signum);
void	signal_handler_cmd(int signum);
int		set_parent_signal(t_env *env);

/* ----- ENV ----- */
/* save_env.c */
int		save_env(t_env *env, char **envp);

/* save_env_utils.c */
int		get_export_vars(t_env *env);
int		cwd_env_error(void);

/* update_env_var.c */
int		set_env_var(t_env *env, char *arg, char *id);
int		new_var(t_env *env, char *arg, int type);

/* update_shlvl.c */
char	*update_shlvl(char *shlvl, int *flag);

/* env_var_check.c */
bool	var_exist(t_list **head, char *id);

/* ----- SCANNER ----- */
/* scanner.c */
int		scanner(char *line);

/* syntax_error.c */
int		check_rd_error(int *rd);
int		syntax_error(char c);

/* ----- TOKENIZER ----- */
/* tokenizer.c */
int		tokenizer(t_cmd **head, char **cmd);

/* get_cmd_2d_array.c */
char	**get_cmd_2d_array(char **str, int pipe);

/* create_tokens.c */
int		create_token(t_vars *var, char **tokens, char *cmd);
bool	last_token(int flag);

/* ----- EXECUTER ----- */
/* errors.c */
int		write_error(t_env *env, int error_type, char *str);
int		fatal_error(int error_type, int process, t_env *env);
void	error_msg(char *str1, char *str2);

/* execute_pipes.c */
int		exec_pipes(t_env *env, t_cmd **head);

/* execute.c */
int		execute(t_env *env, t_cmd *head, char *path);
int		exec_builtin(t_cmd *head, t_env *env);
int		lst_to_2d_array(t_env *env);

/* exit_status.c */
int		get_exit_status(int status);

/* get_paths.c */
int		check_paths(t_env *env, t_cmd *head);

/* prep_execute.c */
int		executer(t_cmd **head, t_env *env);
int		check_command(t_env *env, t_cmd *head);

/* redirections_in.c */
int		set_fd_in(t_cmd *cmd, t_env *env, int fd_in_count);

/* redirections_io_stream.c */
int		set_rd_io_stream(t_cmd *head, t_env *env);
int		reset_rd_io_stream(t_cmd *head, t_env *env);

/* redirections_out.c */
int		set_fd_out(t_cmd *exp, t_env *env, int *fd_out_count);

/* run_shell_cmd.c */
int		run_shell_cmd(t_env *env, t_cmd *head);

/* ----- LEXER ----- */

/* lexer.c */
t_cmd	*lexer(t_cmd **head, char **tokens);

/* set_args.c */
int		set_args(t_vars *var, t_cmd **new, char **tokens, int *arg_count);

/* set_rd.c */
int		set_fn(t_vars *var, t_cmd **new, char **tokens);
int		set_rd(t_vars *var, t_cmd **new, char **tokens);

/* set_token_id.c */
int		set_token_id(t_vars *var, t_cmd **new, char **tokens, int *arg_count);
int		set_expression(t_vars *var, t_cmd **new, int arg_count);

/* ----- HELPER FUNCTIONS ----- */

/* free.c */
char	**free_2d_array(char ***array);
t_cmd	*free_tokens(t_cmd **head);
void	free_str(char **str);
int		free_and_ret(char **str, int ret);

/* identify.c */
bool	fn_token(int flag);
bool	is_builtin(char *name);
bool	is_dot(char *cmd);
bool	is_escape(char c);

/* prep_input.c */
int		prep_input(t_env *env, char *line);
char	*trim_spaces(char *line);

/* ----- BUILTIN ----- */

/* ft_cd.c */
int		ft_cd(t_cmd *head, t_env *env);

/* ft_echo.c */
void	ft_echo(t_cmd *head);

/* ft_env.c */
int		ft_env(t_env *env, t_cmd *head);

/* ft_exit.c */
int		ft_exit(t_cmd *exp);

/* ft_export.c */
int		ft_export(t_cmd *exp, t_env *env);
int		export_var(t_env *env, char *arg);

/* ft_pwd.c */
int		ft_pwd(t_env *env);

/* ft_unset.c */
int		ft_unset(t_cmd *exp, t_env *env);
int		export_unset(t_env *env, char *id, char *var);
void	unset_var(t_list **head, char *id);
int		export_unset_error(char *cmd, char *arg);

/* ----- BUILTIN_UTILS ----- */

/* ft_export_utils.c */
char	*insert_double_quotes(char *str);
void	sort_export_vars(t_list **head, t_list *new);

/* ft_cd_utils.c */
int		update_pwd(t_env *env);
int		cd_error(char **str, int ret);
int		write_cd_error(t_env *env, char *str, int error_type);

/* ----- EXPANSIONS ----- */

/* expand_variable.c */
char	*expand_variable(t_env *env, t_vars *var, char *cmd);

/* expander.c */
char	*expander(t_env *env, char *cmd, int *empty_flag, int pipe);

/* expansion_checks.c */
bool	has_expansion(char *token);
bool	is_home_expansion(char *token, int i);
bool	is_expansion(t_quote *state, char *token, int i);

/* extract_value.c */
char	*extract_value(t_env *env, char *name, char c);
char	*get_var_value(t_env *env, char *var_name);
int		get_len_substr(char *str, int i, t_vars *var);

/* special_parameters.c */
char	*expand_special_param(t_vars *var, char *cmd);

# endif
#endif
