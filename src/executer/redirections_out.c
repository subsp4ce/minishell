#include "../../includes/minishell.h"

int	file_is_stdout(t_env *env, t_cmd *cmd)
{
	cmd->saved_fd_out = dup(STDOUT_FILENO);
	if (cmd->saved_fd_out == ERROR)
		return (fatal_error(DUP_ERROR, PROCESS, env));
	if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR)
		return (fatal_error(DUP2_ERROR, PROCESS, env));
	return (SUCCESS);
}

int	open_close_files(t_rd *rd_head, t_cmd *cmd, t_env *env, int *fd_out_count)
{
	if (rd_head->type == OUT || rd_head->type == DBL_OUT)
	{
		if (rd_head->type == OUT && rd_head->fn != NULL)
			cmd->fd_out = open(rd_head->fn, O_CREAT | O_RDWR | O_TRUNC, 0777);
		else if (rd_head->type == DBL_OUT && rd_head->fn != NULL)
			cmd->fd_out = open(rd_head->fn, O_CREAT | O_RDWR | O_APPEND, 0777);
		if (cmd->fd_out == ERROR)
			return (write_error(env, OPEN_ERROR, rd_head->fn));
		(*fd_out_count)--;
		if (*fd_out_count != 0)
		{
			if (close(cmd->fd_out) == ERROR)
				return (fatal_error(CLOSE_ERROR, PROCESS, env));
		}
	}
	return (SUCCESS);
}

int	set_fd_out(t_cmd *cmd, t_env *env, int *fd_out_count)
{
	t_rd	*rd_head;
	int		ret;

	rd_head = cmd->rd_head;
	while (rd_head != NULL)
	{
		ret = open_close_files(rd_head, cmd, env, fd_out_count);
		if (ret == ERROR || ret == FATAL_ERROR)
			return (ret);
		rd_head = rd_head->next;
	}
	if (cmd->fd_out != 0 && cmd->name == NULL)
	{
		if (close(cmd->fd_out) == ERROR)
			return (fatal_error(CLOSE_ERROR, PROCESS, env));
	}
	else if (cmd->fd_out != 0)
	{
		if (file_is_stdout(env, cmd) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	return (SUCCESS);
}
