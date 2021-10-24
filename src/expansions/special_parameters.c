#include "../../includes/minishell.h"

void	special_parameter_warning(void)
{
	write(1, "Special parameters such as $* $@ $# $- $$ $! $0 ", 48);
	write(1, "are not supported by Minishell and may ", 39);
	write(1, "occur undefined behaviour.\n", 28);
}

char	*expand_special_param(t_vars *var, char *cmd)
{
	char	*value;

	value = NULL;
	if (ft_strchr("*@#-$!0", cmd[var->i + 1]) && cmd[var->i + 1] != '\0')
	{
		special_parameter_warning();
		value = ft_strdup("");
	}
	else if (ft_isdigit(cmd[var->i + 1]))
	{
		var->i++;
		value = ft_strdup("");
	}
	else if (cmd[var->i + 1] == '?')
	{
		var->i++;
		value = ft_itoa(g_exit_status);
	}
	else
		value = ft_strdup("$");
	if (value == NULL)
		return (NULL);
	return (value);
}
