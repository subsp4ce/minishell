#include "../../includes/minishell.h"

int	file_is_stdin(t_cmd *cmd, t_env *env)
{
	cmd->saved_fd_in = dup(STDIN_FILENO);
	if (cmd->saved_fd_in == ERROR)
		return (fatal_error(DUP_ERROR, PROCESS, env));
	if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR)
		return (fatal_error(DUP2_ERROR, PROCESS, env));
	return (SUCCESS);
}

int	fd_in_found(t_cmd *cmd, t_rd *rd_head, t_env *env, int *fd_in_count)
{
	cmd->fd_in = open(rd_head->fn, O_RDONLY, 0777);
	if (cmd->fd_in == ERROR)
		return (write_error(env, OPEN_ERROR, rd_head->fn));
	(*fd_in_count)--;
	if (*fd_in_count != 0)
	{
		if (close(cmd->fd_in) == ERROR)
			return (fatal_error(CLOSE_ERROR, PROCESS, env));
	}
	return (SUCCESS);
}

int	set_fd_in(t_cmd *cmd, t_env *env, int fd_in_count)
{
	t_rd	*rd_head;
	t_rd	*last_in;
	int		ret;

	rd_head = cmd->rd_head;
	last_in = NULL;
	ret = 0;
	while (rd_head != NULL)
	{
		if (rd_head->type == IN)
		{
			ret = fd_in_found(cmd, rd_head, env, &fd_in_count);
			if (ret == ERROR || ret == FATAL_ERROR)
				return (ret);
			last_in = rd_head;
		}
		rd_head = rd_head->next;
	}
	if (last_in != NULL && cmd->name != NULL)
	{
		if (file_is_stdin(cmd, env) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	return (SUCCESS);
}
