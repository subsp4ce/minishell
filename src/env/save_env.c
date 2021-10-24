#include "../../includes/minishell.h"

bool	env_var_exists(t_env *env, char *var)
{
	t_list	*lst;

	lst = env->vars;
	while (lst != NULL)
	{
		if (ft_strncmp(lst->content, var, ft_strlen(var)) == 0)
			return (true);
		lst = lst->next;
	}
	return (false);
}

int	add_pwd(t_env *env)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (cwd_env_error());
	pwd = ft_strjoin("PWD=", cwd);
	if (pwd == NULL)
	{
		free(cwd);
		return (FATAL_ERROR);
	}
	if (new_var(env, pwd, ENV) == FATAL_ERROR)
	{
		free(cwd);
		free(pwd);
		return (FATAL_ERROR);
	}
	free(cwd);
	free(pwd);
	return (SUCCESS);
}

int	handle_special_env_vars(t_env *env)
{
	unset_var(&env->vars, "OLDPWD");
	if (env->flag == OFF)
	{
		if (new_var(env, "SHLVL=1", ENV) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	if (!env_var_exists(env, "PWD="))
	{
		if (add_pwd(env) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	env->saved_pwd = get_var_value(env, "PWD=");
	if (env->saved_pwd == NULL)
		return (FATAL_ERROR);
	env->flag = OFF;
	return (SUCCESS);
}

int	get_env_vars(t_env *env, char **envp)
{
	t_list	*new;
	char	*var;
	int		i;

	new = NULL;
	var = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", ft_strlen("SHLVL=")) == 0)
			var = update_shlvl(envp[i], &env->flag);
		else
			var = ft_strdup(envp[i]);
		if (var == NULL)
			return (FATAL_ERROR);
		new = ft_lstnew(var);
		if (new == NULL)
		{
			free(var);
			return (FATAL_ERROR);
		}
		ft_lstadd_back(&env->vars, new);
		i++;
	}
	return (SUCCESS);
}

int	save_env(t_env *env, char **envp)
{
	if (envp == NULL)
		return (SUCCESS);
	if (get_env_vars(env, envp) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (handle_special_env_vars(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (get_export_vars(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	return (SUCCESS);
}
