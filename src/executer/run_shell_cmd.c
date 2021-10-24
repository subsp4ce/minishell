#include "../../includes/minishell.h"

bool	path_has_value(t_env *env)
{
	t_list	*lst;

	lst = env->vars;
	while (lst != NULL)
	{
		if ((ft_strncmp(lst->content, "PATH=", 5) == 0)
			&& ft_strlen(lst->content) != 5)
			return (true);
		lst = lst->next;
	}
	return (false);
}

int	run_shell_cmd(t_env *env, t_cmd *head)
{
	int		ret;

	if (path_has_value(env))
	{
		ret = check_paths(env, head);
		if (ret == ERROR || ret == FATAL_ERROR)
			return (ret);
	}
	else
	{
		if (head->operator == '|')
		{
			if (dup2(env->saved_stdout, STDOUT_FILENO) == ERROR)
				return (fatal_error(DUP2_ERROR, PROCESS, env));
		}
		error_msg(head->name, ": No such file or directory");
		return (get_exit_status(127));
	}
	return (SUCCESS);
}
