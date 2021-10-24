#include "../../includes/minishell.h"

int	export_pwd(t_env *env, char *cwd)
{
	char	*pwd;
	int		ret;

	pwd = ft_strjoin("PWD=", cwd);
	if (pwd == NULL)
		return (FATAL_ERROR);
	if (var_exist(&env->exp, "PWD"))
	{
		ret = export_var(env, pwd);
		if (ret == FATAL_ERROR || ret == ERROR)
		{
			free(pwd);
			return (ret);
		}
	}
	free(pwd);
	return (SUCCESS);
}

/*
**	getcwd()
**	If a buffer is specified, the path name is placed in
**	that buffer, and the address of the buffer is returned.
**	Otherwise, getcwd() allocates space for the path name and
**	returns a pointer to that buffer.
*/

int	update_pwd(t_env *env)
{
	char	*cwd;
	int		ret;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (write_error(env, GETCWD_ERROR, "getcwd"));
	ret = export_pwd(env, cwd);
	if (ret == FATAL_ERROR || ret == ERROR)
		return (free_and_ret(&cwd, ret));
	free(env->saved_pwd);
	env->saved_pwd = ft_strdup(cwd);
	if (env->saved_pwd == NULL)
		return (free_and_ret(&cwd, FATAL_ERROR));
	free(cwd);
	return (SUCCESS);
}

int	write_cd_error(t_env *env, char *str, int error_type)
{
	if (check_o_stream(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (error_type == HOME_ERROR)
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
	if (error_type == CHDIR_ERROR)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (get_exit_status(1));
}
