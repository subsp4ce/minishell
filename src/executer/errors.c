#include "../../includes/minishell.h"

void	write_fatal_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

int	fatal_error(int error_type, int process, t_env *env)
{
	if (check_o_stream(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (error_type == CLOSE_ERROR)
		write_fatal_error("close failed: ");
	else if (error_type == DUP2_ERROR)
		write_fatal_error("dup2 failed: ");
	else if (error_type == DUP_ERROR)
		write_fatal_error("dup failed: ");
	else if (error_type == PIPE_ERROR)
		write_fatal_error("pipe failed: ");
	else if (error_type == FORK_ERROR)
		write_fatal_error("fork failed: ");
	else if (error_type == WAITPID_ERROR)
		write_fatal_error("waitpid failed: ");
	else if (error_type == STAT_ERROR)
		write_fatal_error("stat failed: ");
	else if (error_type == SIGNAL_ERROR)
		write_fatal_error("signal error: ");
	if (process == CHILD)
		exit(g_exit_status);
	return (FATAL_ERROR);
}

int	write_error(t_env *env, int error_type, char *str)
{
	if (check_o_stream(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (error_type == OPEN_ERROR || error_type == STAT_ERROR
		|| error_type == GETCWD_ERROR)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (error_type == OPEN_ERROR)
		g_exit_status = 1;
	else if (errno == 2)
		g_exit_status = 127;
	else if (errno == 13)
		g_exit_status = 126;
	else
		g_exit_status = 1;
	return (ERROR);
}

void	error_msg(char *str1, char *str2)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str1, STDERR_FILENO);
	if (str2[0] != '\0')
		ft_putstr_fd(str2, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
