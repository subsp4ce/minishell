#include "../../includes/minishell.h"

/*
**	pipe() then fork() - child process is now a copy of the parent
**	first child reads from stdin
**	each subsequent child reads from prior READ side of pipe - parent stores
**	the READ side of each pipe in a var and passes it to next child
**	child writes to WRITE side of pipe, except last child writes to stdout
**	waitpid() - iterate through nodes and wait for each child to finish
** 	in case of infinite process, a signal needs to be passed to stop process,
**	child will die and wait will catch this signal
*/

int	wait_processes(t_cmd *head, t_env *env)
{
	int	stat_loc;

	while (head != NULL)
	{
		if (waitpid(head->pid, &stat_loc, WUNTRACED) == ERROR)
			return (fatal_error(WAITPID_ERROR, PROCESS, env));
		if (WIFSIGNALED(stat_loc))
			g_exit_status = (WTERMSIG(stat_loc) + 128);
		else
			get_exit_status(stat_loc);
		head = head->next;
	}
	return (SUCCESS);
}

int	parent_pipe(t_cmd *head, t_env *env)
{
	if (head->operator == '|')
	{
		if (close(head->pipe_fd[WRITE]) == ERROR)
			return (fatal_error(CLOSE_ERROR, PROCESS, env));
		env->read_fd = head->pipe_fd[READ];
	}
	return (SUCCESS);
}

void	child_pipe(t_cmd *head, t_env *env)
{
	int	ret;

	ret = 0;
	reset_sig(env);
	change_pipe_streams(head, env);
	ret = check_command(env, head);
	if (ret == FATAL_ERROR)
		fatal_error(FATAL_ERROR, CHILD, env);
	else if (ret == SUCCESS)
		g_exit_status = 0;
	exit(g_exit_status);
}

int	pipe_cmd(t_cmd **head, t_env *env)
{
	if ((*head)->operator == '|')
	{
		if (pipe((*head)->pipe_fd) == ERROR)
			return (fatal_error(PIPE_ERROR, PROCESS, env));
	}
	(*head)->pid = fork();
	if ((*head)->pid == ERROR)
		return (fatal_error(FORK_ERROR, PROCESS, env));
	else if ((*head)->pid == 0)
		child_pipe(*head, env);
	else if (parent_pipe(*head, env) == FATAL_ERROR)
		return (FATAL_ERROR);
	return (SUCCESS);
}

int	exec_pipes(t_env *env, t_cmd **head)
{
	t_cmd	*ptr;

	ptr = *head;
	env->read_fd = 0;
	env->is_pipe = ON;
	if (set_parent_signal(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	while (ptr != NULL)
	{
		if (pipe_cmd(&ptr, env) == FATAL_ERROR)
			return (FATAL_ERROR);
		if (ptr->prev != NULL)
		{
			if (ptr->prev->operator == '|')
			{
				if (close(ptr->prev->pipe_fd[READ]) == ERROR)
					return (fatal_error(CLOSE_ERROR, PROCESS, env));
			}
		}
		ptr = ptr->next;
	}
	if (wait_processes(*head, env) == FATAL_ERROR)
		return (FATAL_ERROR);
	env->is_pipe = OFF;
	return (SUCCESS);
}
