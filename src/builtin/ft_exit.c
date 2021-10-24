#include "../../includes/minishell.h"

int	write_exit_error(char *arg, int type)
{
	if (type == 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd("numeric argument required", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		g_exit_status = 255;
	}
	else if (type == 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_exit_status = 1;
		return (ERROR);
	}
	return (g_exit_status);
}

bool	is_all_digit_neg(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-' && i == 0)
			i++;
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_exit(t_cmd *exp)
{
	int	count;

	count = 0;
	if (exp->prev != NULL)
	{
		if (exp->prev->operator != '|')
			ft_putstr_fd("exit\n", STDERR_FILENO);
	}
	else if (exp->operator != '|')
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (exp->args != NULL)
	{
		while (exp->args[count])
			count++;
		if (is_all_digit_neg(exp->args[0]) && count == 1)
			g_exit_status = ft_atoi(exp->args[0]);
		else if (is_all_digit_neg(exp->args[0]) && count > 1)
			return (write_exit_error(exp->args[0], 2));
		else if (!is_all_digit_neg(exp->args[0]))
			exit(write_exit_error(exp->args[0], 1));
	}
	exit(g_exit_status);
}
