#include "../../includes/minishell.h"

int	check_directory(t_env *env, char *path)
{
	struct stat	stats;
	int			ret;

	ret = stat(path, &stats);
	if (ret == ERROR)
		return (write_error(env, STAT_ERROR, path));
	else if (S_ISDIR(stats.st_mode))
	{
		g_exit_status = 126;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		return (ERROR);
	}
	return (SUCCESS);
}

int	check_cmd_type(t_env *env, t_cmd *head)
{
	int	ret;

	ret = 0;
	if (ft_strchr(head->name, '/'))
	{
		ret = check_directory(env, head->name);
		if (ret == ERROR || ret == FATAL_ERROR)
			return (ret);
		ret = execute(env, head, head->name);
	}
	else if (is_builtin(head->name))
		ret = exec_builtin(head, env);
	else
		ret = run_shell_cmd(env, head);
	return (ret);
}

int	check_command(t_env *env, t_cmd *head)
{
	int	ret;

	ret = set_rd_io_stream(head, env);
	if (ret == FATAL_ERROR || ret == ERROR)
		return (ret);
	if (head->name != NULL)
		ret = check_cmd_type(env, head);
	if (ret == FATAL_ERROR)
		return (FATAL_ERROR);
	if (reset_rd_io_stream(head, env) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (ret == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	executer(t_cmd **head, t_env *env)
{
	int		ret;

	ret = 0;
	if ((*head)->operator == '|')
	{
		if (save_iostream(env) == FATAL_ERROR)
			return (FATAL_ERROR);
		if (exec_pipes(env, head) == FATAL_ERROR)
			return (FATAL_ERROR);
		if (close_saved_fds(env) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	else
	{
		ret = check_command(env, *head);
		if (ret == FATAL_ERROR)
			return (FATAL_ERROR);
		else if (ret != ERROR)
			g_exit_status = 0;
	}
	return (SUCCESS);
}
