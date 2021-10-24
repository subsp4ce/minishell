#include "../../includes/minishell.h"

int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else
		g_exit_status = status;
	if (g_exit_status != 0)
		return (ERROR);
	else
		return (SUCCESS);
}
