#include "../../includes/minishell.h"

int	lst_to_2d_array(t_env *env)
{
	t_list	*lst;
	int		len;
	int		i;

	len = ft_lstsize(env->vars);
	lst = env->vars;
	i = 0;
	if (env->envp != NULL)
		free_2d_array(&env->envp);
	env->envp = ft_calloc(len + 1, sizeof(char *));
	if (env->envp == NULL)
		return (FATAL_ERROR);
	while (lst != NULL)
	{
		env->envp[i] = ft_strdup(lst->content);
		if (env->envp[i] == NULL)
		{
			free_2d_array(&env->envp);
			return (FATAL_ERROR);
		}
		i++;
		lst = lst->next;
	}
	return (SUCCESS);
}

void	execute_child(t_cmd *head, t_env *env, char *path)
{
	if (lst_to_2d_array(env) == FATAL_ERROR)
		exit(1);
	reset_sig(env);
	execve(path, head->argv, env->envp);
	if (errno == 2)
	{	
		error_msg(path, ": No such file or directory");
		exit(127);
	}
	if (errno == 13)
	{
		error_msg(path, ": Permission denied");
		exit(126);
	}
	exit(1);
}

int	execute_parent(t_cmd *head, t_env *env)
{
	int	stat_loc;

	if (set_parent_signal(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (waitpid(head->pid, &stat_loc, WUNTRACED) == ERROR)
		return (fatal_error(WAITPID_ERROR, PROCESS, env));
	if (WIFSIGNALED(stat_loc))
	{
		g_exit_status = (WTERMSIG(stat_loc) + 128);
		return (ERROR);
	}
	return (get_exit_status(stat_loc));
}

/*
**	If env->is_pipe is ON, we are in a child process and
**	we don't want to fork() again.
*/

int	execute(t_env *env, t_cmd *head, char *path)
{
	int		ret;

	ret = 0;
	if (env->is_pipe == ON)
		execute_child(head, env, path);
	else
	{
		head->pid = fork();
		if (head->pid == ERROR)
			return (fatal_error(FORK_ERROR, PROCESS, env));
		else if (head->pid == 0)
			execute_child(head, env, path);
		else
		{
			ret = execute_parent(head, env);
			if (ret == ERROR || ret == FATAL_ERROR)
				return (ret);
		}
	}
	return (SUCCESS);
}

int	exec_builtin(t_cmd *head, t_env *env)
{
	int	ret;

	ret = 0;
	if (ft_strncmp(head->name, "echo", 4) == 0)
		ft_echo(head);
	else if (ft_strncmp(head->name, "pwd", 3) == 0)
		ret = ft_pwd(env);
	else if (ft_strncmp(head->name, "cd", 2) == 0)
		ret = ft_cd(head, env);
	else if (ft_strncmp(head->name, "env", 6) == 0)
		ret = ft_env(env, head);
	else if (ft_strncmp(head->name, "export", 6) == 0)
		ret = ft_export(head, env);
	else if (ft_strncmp(head->name, "unset", 4) == 0)
		ret = ft_unset(head, env);
	else if (ft_strncmp(head->name, "exit", 4) == 0)
		ret = ft_exit(head);
	if (ret == ERROR || ret == FATAL_ERROR)
		return (ret);
	return (SUCCESS);
}
