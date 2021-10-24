#include "../includes/minishell.h"

int	save_iostream(t_env *env)
{
	env->saved_stdin = dup(STDIN_FILENO);
	if (env->saved_stdin == ERROR)
		return (fatal_error(DUP_ERROR, PROCESS, env));
	env->saved_stdout = dup(STDOUT_FILENO);
	if (env->saved_stdout == ERROR)
		return (fatal_error(DUP_ERROR, PROCESS, env));
	return (SUCCESS);
}

int	close_saved_fds(t_env *env)
{
	if (close(env->saved_stdin) == ERROR)
		return (fatal_error(CLOSE_ERROR, PROCESS, env));
	if (close(env->saved_stdout) == ERROR)
		return (fatal_error(CLOSE_ERROR, PROCESS, env));
	return (SUCCESS);
}

void	change_pipe_streams(t_cmd *head, t_env *env)
{
	if (env->read_fd != 0)
	{
		if (dup2(env->read_fd, STDIN_FILENO) == ERROR)
			fatal_error(DUP2_ERROR, CHILD, env);
	}
	if (head->operator == '|')
	{
		if (dup2(head->pipe_fd[WRITE], STDOUT_FILENO) == ERROR)
			fatal_error(DUP2_ERROR, CHILD, env);
	}
	else
	{
		if (dup2(env->saved_stdout, STDOUT_FILENO) == ERROR)
			fatal_error(DUP2_ERROR, CHILD, env);
	}
	if (head->operator == '|')
	{
		if (close(head->pipe_fd[WRITE]) == ERROR)
			fatal_error(CLOSE_ERROR, CHILD, env);
		if (close(head->pipe_fd[READ]) == ERROR)
			fatal_error(CLOSE_ERROR, CHILD, env);
	}
}

int	check_o_stream(t_env *env)
{
	if (env->is_pipe == ON)
	{
		if (dup2(env->saved_stdout, STDOUT_FILENO) == ERROR)
			return (fatal_error(DUP2_ERROR, PROCESS, env));
		env->is_pipe = 0;
	}
	return (SUCCESS);
}
