#include "../../includes/minishell.h"

int	cd_no_args(t_env *env)
{
	char	*home;

	home = NULL;
	if (var_exist(&env->vars, "HOME"))
	{
		home = get_var_value(env, "HOME=");
		if (home == NULL)
			return (FATAL_ERROR);
		if (ft_strncmp(home, "", ft_strlen(home)) == 0)
		{
			free(home);
			return (SUCCESS);
		}
	}
	else
		return (write_cd_error(env, NULL, HOME_ERROR));
	if (chdir(home) == ERROR)
	{
		free(home);
		return (write_cd_error(env, home, CHDIR_ERROR));
	}
	free(home);
	return (SUCCESS);
}

int	exec_cd(t_env *env, t_cmd *head)
{
	if (head->args == NULL)
		return (cd_no_args(env));
	else
	{
		if (chdir(head->args[0]) == ERROR)
			return (write_cd_error(env, head->args[0], CHDIR_ERROR));
	}
	return (SUCCESS);
}

char	*get_oldpwd(t_env *env)
{
	char	*oldpwd;
	char	*tmp;

	tmp = ft_strdup(env->saved_pwd);
	if (tmp == NULL)
		return (NULL);
	oldpwd = ft_strjoin("OLDPWD=", tmp);
	if (oldpwd == NULL)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (oldpwd);
}

int	ft_cd(t_cmd *head, t_env *env)
{
	char	*oldpwd;
	int		ret;

	oldpwd = get_oldpwd(env);
	if (oldpwd == NULL)
		return (FATAL_ERROR);
	ret = exec_cd(env, head);
	if (ret == FATAL_ERROR || ret == ERROR)
		return (free_and_ret(&oldpwd, ret));
	ret = update_pwd(env);
	if (ret == FATAL_ERROR || ret == ERROR)
		return (free_and_ret(&oldpwd, ret));
	if (var_exist(&env->exp, "OLDPWD"))
	{
		ret = export_var(env, oldpwd);
		if (ret == FATAL_ERROR || ret == ERROR)
			return (free_and_ret(&oldpwd, ret));
	}
	free(oldpwd);
	return (get_exit_status(0));
}
