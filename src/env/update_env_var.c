#include "../../includes/minishell.h"

int	find_and_replace_var(t_env *env, char *id, char *var)
{
	t_list	*lst;
	int		len;
	int		i;

	lst = env->vars;
	len = ft_strlen(id);
	while (lst != NULL)
	{
		i = 0;
		while (lst->content[i] != '=' && lst->content[i] != '\0')
			i++;
		if (ft_strncmp(lst->content, id, len) == 0 && i == len)
		{
			free(lst->content);
			lst->content = ft_strdup(var);
			if (lst->content == NULL)
				return (FATAL_ERROR);
			return (SUCCESS);
		}
		lst = lst->next;
	}
	return (SUCCESS);
}

int	new_var(t_env *env, char *arg, int type)
{
	t_list	*new;
	char	*var;

	new = NULL;
	var = ft_strdup(arg);
	if (var == NULL)
		return (FATAL_ERROR);
	new = ft_lstnew(var);
	if (new == NULL)
	{
		free(var);
		return (FATAL_ERROR);
	}
	if (type == EXPORT)
		sort_export_vars(&env->exp, new);
	else if (type == ENV)
		ft_lstadd_back(&env->vars, new);
	return (SUCCESS);
}

int	add_export_env_var(t_env *env, char *arg, char *id)
{
	char	*var;

	unset_var(&env->exp, id);
	var = insert_double_quotes(arg);
	if (var == NULL)
		return (FATAL_ERROR);
	if (new_var(env, var, EXPORT) == FATAL_ERROR)
		return (free_and_ret(&var, FATAL_ERROR));
	if (var_exist(&env->vars, id))
	{
		if (find_and_replace_var(env, id, arg) == FATAL_ERROR)
			return (free_and_ret(&var, FATAL_ERROR));
	}
	else
	{
		if (new_var(env, arg, ENV) == FATAL_ERROR)
			return (free_and_ret(&var, FATAL_ERROR));
	}
	free(var);
	return (SUCCESS);
}

/*
**	First check protects from overwriting an export variable that has a value
*/

int	add_export_var_no_value(t_env *env, char *arg, char *id)
{	
	char	*var;

	var = NULL;
	if (var_exist(&env->exp, id))
		return (SUCCESS);
	var = ft_strdup(arg);
	if (var == NULL)
		return (FATAL_ERROR);
	new_var(env, var, EXPORT);
	free(var);
	return (SUCCESS);
}

/*
**	Add a variable to export variables only or
**	to both export and environment variables.
*/

int	set_env_var(t_env *env, char *arg, char *id)
{
	if (!ft_strchr(arg, '='))
	{
		if (add_export_var_no_value(env, arg, id) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	else
	{
		if (add_export_env_var(env, arg, id) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	return (SUCCESS);
}
