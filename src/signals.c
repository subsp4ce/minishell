#include "../includes/minishell.h"

int	set_parent_signal(t_env *env)
{
	if (signal(SIGINT, signal_handler_cmd) == SIG_ERR)
		return (fatal_error(SIGNAL_ERROR, PROCESS, env));
	if (signal(SIGQUIT, signal_handler_cmd) == SIG_ERR)
		return (fatal_error(SIGNAL_ERROR, PROCESS, env));
	return (SUCCESS);
}

/*
**	Handle signals without any processes running
*/

int	catch_signals(t_env *env)
{
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		return (fatal_error(SIGNAL_ERROR, PROCESS, env));
	if (signal(SIGQUIT, signal_handler) == SIG_ERR)
		return (fatal_error(SIGNAL_ERROR, PROCESS, env));
	return (SUCCESS);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\b\b  \n", 5);
		g_exit_status = 1;
		prompt();
	}
	if (signum == SIGQUIT)
		write(STDOUT_FILENO, "\b\b  \b\b", 6);
}

/*
**	Reset signals to their defaults in the child processes
*/

int	reset_sig(t_env *env)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		return (fatal_error(SIGNAL_ERROR, CHILD, env));
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		return (fatal_error(SIGNAL_ERROR, CHILD, env));
	return (SUCCESS);
}

/*
**	Prints a newline or Quit before killing the child process
*/

void	signal_handler_cmd(int signum)
{
	if (signum == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	if (signum == SIGQUIT)
		write(STDOUT_FILENO, "Quit: 3\n", 8);
}
