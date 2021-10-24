#include "../../includes/minishell.h"

int	execute_shell_cmd(t_env *env, t_cmd *head, t_vars *var)
{
	if (head->operator == '|')
	{
		if (lst_to_2d_array(env) == FATAL_ERROR)
			exit(1);
		execve(env->paths[var->i], head->argv, env->envp);
		if (errno == 2)
		{	
			error_msg(env->paths[var->i], ": No such file or directory");
			exit(127);
		}
		if (errno == 13)
		{
			error_msg(env->paths[var->i], ": Permission denied");
			exit(126);
		}
		exit(1);
	}
	else
	{
		var->ret = execute(env, head, env->paths[var->i]);
		if (var->ret == ERROR || var->ret == FATAL_ERROR)
			return (var->ret);
	}
	return (get_exit_status(SUCCESS));
}

int	find_path(t_env *env, t_cmd *head)
{
	t_vars		var;
	struct stat	stats;

	ft_bzero(&var, sizeof(t_vars));
	while (env->paths[var.i] && ft_strlen(head->name) != 0
		&& !is_dot(head->name))
	{
		if (stat(env->paths[var.i], &stats) == 0)
			return (execute_shell_cmd(env, head, &var));
		var.i++;
	}
	if (head->operator == '|')
	{
		if (dup2(env->saved_stdout, STDOUT_FILENO) == ERROR)
			return (fatal_error(DUP2_ERROR, PROCESS, env));
	}
	error_msg(head->argv[0], ": command not found");
	return (get_exit_status(127));
}

char	**concat_cmd(char **path_array, char *cmd)
{
	char	**paths;
	t_vars	var;
	int		len_cmd;

	ft_bzero(&var, sizeof(t_vars));
	while (path_array[var.i])
		var.i++;
	paths = ft_calloc(var.i + 1, sizeof(char *));
	if (paths == NULL)
		return (NULL);
	var.i = 0;
	while (path_array[var.i])
	{
		var.len = ft_strlen(path_array[var.i]);
		len_cmd = ft_strlen(cmd);
		paths[var.i] = ft_calloc(var.len + len_cmd + 2, sizeof(char));
		if (paths[var.i] == NULL)
			return (free_2d_array(&paths));
		ft_strlcpy(paths[var.i], path_array[var.i], var.len + 1);
		ft_strlcat(paths[var.i], "/", var.len + 2);
		ft_strlcat(paths[var.i], cmd, var.len + len_cmd + 2);
		var.i++;
	}
	return (paths);
}

char	*get_paths(t_env *env)
{
	t_list	*lst;

	lst = env->vars;
	while (lst != NULL)
	{
		if (ft_strncmp(lst->content, "PATH=", 5) == 0)
			break ;
		lst = lst->next;
	}
	return (ft_substr(lst->content, 5, ft_strlen(lst->content) - 4));
}

int	check_paths(t_env *env, t_cmd *head)
{
	char	*path;
	char	**path_array;

	path = get_paths(env);
	if (path == NULL)
		return (FATAL_ERROR);
	path_array = ft_split(path, ':');
	if (path_array == NULL)
	{
		free(path);
		return (FATAL_ERROR);
	}
	if (env->paths != NULL)
		free_2d_array(&env->paths);
	env->paths = concat_cmd(path_array, head->argv[0]);
	if (env->paths == NULL)
	{
		free(path);
		free_2d_array(&path_array);
		return (FATAL_ERROR);
	}
	free(path);
	free_2d_array(&path_array);
	return (find_path(env, head));
}
