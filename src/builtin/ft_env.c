#include "../../includes/minishell.h"

int	ft_env(t_env *env, t_cmd *head)
{
	t_list	*lst;

	lst = env->vars;
	if (head->args != NULL)
	{
		error_msg("env: ", "arguments not supported");
		return (get_exit_status(1));
	}
	while (lst != NULL)
	{
		ft_putstr_fd(lst->content, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		lst = lst->next;
	}
	return (get_exit_status(0));
}
