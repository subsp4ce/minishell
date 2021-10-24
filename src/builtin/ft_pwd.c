#include "../../includes/minishell.h"

int	ft_pwd(t_env *env)
{
	ft_putstr_fd(env->saved_pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (get_exit_status(0));
}
