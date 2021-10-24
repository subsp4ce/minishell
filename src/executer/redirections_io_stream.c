#include "../../includes/minishell.h"

int	reset_rd_io_stream(t_cmd *head, t_env *env)
{
	if (head->fd_in != 0 && head->name != NULL)
	{
		if (dup2(head->saved_fd_in, STDIN_FILENO) == ERROR)
			return (fatal_error(DUP2_ERROR, PROCESS, env));
		if (close(head->saved_fd_in) == ERROR)
			return (fatal_error(CLOSE_ERROR, PROCESS, env));
		if (close(head->fd_in) == ERROR)
			return (fatal_error(CLOSE_ERROR, PROCESS, env));
	}
	if (head->fd_out != 0 && head->name != NULL)
	{
		if (dup2(head->saved_fd_out, STDOUT_FILENO) == ERROR)
			return (fatal_error(DUP2_ERROR, PROCESS, env));
		if (close(head->saved_fd_out) == ERROR)
			return (fatal_error(CLOSE_ERROR, PROCESS, env));
		if (close(head->fd_out) == ERROR)
			return (fatal_error(CLOSE_ERROR, PROCESS, env));
	}
	return (SUCCESS);
}

int	set_rd_io_stream(t_cmd *head, t_env *env)
{
	t_rd	*rd_head;
	int		fd_in_count;
	int		fd_out_count;
	int		ret;

	rd_head = head->rd_head;
	fd_in_count = 0;
	fd_out_count = 0;
	while (rd_head != NULL)
	{
		if (rd_head->type == IN)
			fd_in_count++;
		if (rd_head->type == OUT || rd_head->type == DBL_OUT)
			fd_out_count++;
		rd_head = rd_head->next;
	}
	ret = set_fd_in(head, env, fd_in_count);
	if (ret == FATAL_ERROR || ret == ERROR)
		return (ret);
	ret = (set_fd_out(head, env, &fd_out_count));
	if (ret == FATAL_ERROR || ret == ERROR)
		return (ret);
	return (SUCCESS);
}
