#include "../../includes/minishell.h"

int	export_unset_error(char *cmd, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (get_exit_status(1));
}

void	del_var(t_list **head, t_list *lst)
{
	if (lst->prev == NULL && lst->next == NULL)
	{
		*head = NULL;
		return (ft_lstdelone(lst, &free));
	}
	if (lst->prev == NULL && lst->next != NULL)
	{
		*head = lst->next;
		(*head)->prev = NULL;
		return (ft_lstdelone(lst, &free));
	}
	else if (lst->prev != NULL && lst->next != NULL)
	{
		lst->prev->next = lst->next;
		lst->next->prev = lst->prev;
		return (ft_lstdelone(lst, &free));
	}
	else if (lst->prev != NULL && lst->next == NULL)
	{
		lst->prev->next = NULL;
		return (ft_lstdelone(lst, &free));
	}
}

void	unset_var(t_list **head, char *id)
{
	t_list	*lst;
	int		len;
	int		i;

	lst = *head;
	len = ft_strlen(id);
	while (lst != NULL)
	{
		i = 0;
		while (lst->content[i] != '=' && lst->content[i] != '\0')
			i++;
		if (ft_strncmp(lst->content, id, len) == 0 && i == len)
			return (del_var(head, lst));
		lst = lst->next;
	}
}

int	validate_unset(char *arg)
{
	int	i;

	i = 0;
	if (ft_isdigit(arg[i]))
		return (export_unset_error("unset", arg));
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (export_unset_error("unset", arg));
		i++;
	}
	return (SUCCESS);
}

int	ft_unset(t_cmd *cmd, t_env *env)
{
	int	flag;
	int	i;

	flag = 0;
	i = 0;
	if (cmd->args != NULL)
	{
		while (cmd->args[i] != NULL)
		{
			if (validate_unset(cmd->args[i]) == ERROR)
				flag = ON;
			unset_var(&env->vars, cmd->args[i]);
			unset_var(&env->exp, cmd->args[i]);
			i++;
		}
	}
	if (flag == ON)
		return (ERROR);
	return (SUCCESS);
}
