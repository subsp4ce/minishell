#include "../../includes/minishell.h"

void	print_export_vars(t_list *exp_vars)
{
	t_list	*head;

	head = exp_vars;
	while (head != NULL)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(head->content, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		head = head->next;
	}
}

int	validate_export(char *arg, char *id)
{
	int	i;

	i = 0;
	if (id == NULL)
		return (export_unset_error("export", arg));
	if (ft_isdigit(id[i]))
		return (export_unset_error("export", arg));
	if (arg != NULL)
	{
		if (arg[i] == '=')
			return (export_unset_error("export", arg));
	}
	while (id[i])
	{
		if (!ft_isalnum(id[i]) && id[i] != '_')
			return (export_unset_error("export", arg));
		i++;
	}
	return (SUCCESS);
}

int	export_var(t_env *env, char *arg)
{
	char	**arg_split;

	arg_split = ft_split(arg, '=');
	if (arg_split == NULL)
		return (FATAL_ERROR);
	if (validate_export(arg, arg_split[0]) == ERROR)
	{
		free_2d_array(&arg_split);
		return (ERROR);
	}
	if (set_env_var(env, arg, arg_split[0]) == FATAL_ERROR)
	{
		free_2d_array(&arg_split);
		return (FATAL_ERROR);
	}
	free_2d_array(&arg_split);
	return (SUCCESS);
}

int	ft_export(t_cmd *exp, t_env *env)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	if (exp->args != NULL)
	{
		while (exp->args[i] != NULL)
		{
			ret = export_var(env, exp->args[i]);
			i++;
		}
	}
	else
		print_export_vars(env->exp);
	if (ret == ERROR || ret == FATAL_ERROR)
		return (ret);
	return (SUCCESS);
}
