#include "../../includes/minishell.h"

int	cwd_env_error(void)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("cwd", STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (FATAL_ERROR);
}

int	handle_oldpwd(t_env *env)
{
	t_list	*new;
	char	*oldpwd;

	new = NULL;
	oldpwd = ft_strdup("OLDPWD");
	if (oldpwd == NULL)
		return (FATAL_ERROR);
	unset_var(&env->exp, "OLDPWD");
	new = ft_lstnew(oldpwd);
	if (new == NULL)
	{
		free(oldpwd);
		return (FATAL_ERROR);
	}
	sort_export_vars(&env->exp, new);
	return (SUCCESS);
}

int	get_export_vars(t_env *env)
{
	t_list	*lst;
	t_list	*new;
	char	*var;

	lst = env->vars;
	new = NULL;
	var = NULL;
	while (lst != NULL)
	{
		var = insert_double_quotes(lst->content);
		if (var == NULL)
			return (FATAL_ERROR);
		new = ft_lstnew(var);
		if (new == NULL)
		{
			free(var);
			return (FATAL_ERROR);
		}
		sort_export_vars(&env->exp, new);
		lst = lst->next;
	}
	unset_var(&env->exp, "_");
	if (handle_oldpwd(env) == FATAL_ERROR)
		return (FATAL_ERROR);
	return (SUCCESS);
}
