#include "../../includes/minishell.h"

void	write_args(t_cmd *head, int i)
{
	while (head->args[i])
	{
		ft_putstr_fd(head->args[i], STDOUT_FILENO);
		if (head->args[i + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	process_newline(t_cmd *head, int *newline)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (ft_strncmp(head->args[i], "-n", 2) == 0)
	{
		j = 2;
		if (ft_strlen(head->args[i]) == 2)
			*newline = false;
		else
		{
			while (head->args[i][j])
			{
				if (head->args[i][j] != 'n')
					return (i);
				j++;
			}
			*newline = false;
		}
		i++;
		if (head->args[i] == NULL)
			return (i);
	}
	return (i);
}

void	ft_echo(t_cmd *head)
{
	int		i;
	int		newline;

	i = 0;
	newline = true;
	if (head->args == NULL)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	else
		i = process_newline(head, &newline);
	write_args(head, i);
	if (newline == true)
		ft_putchar_fd('\n', STDOUT_FILENO);
	g_exit_status = 0;
	return ;
}
