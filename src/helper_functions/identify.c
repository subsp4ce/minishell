#include "../../includes/minishell.h"

bool	fn_token(int flag)
{
	if (flag == 1)
		return (true);
	return (false);
}

bool	is_escape(char c)
{
	if (c == '\"' || c == '$' || c == '`' || c == '\\')
		return (true);
	return (false);
}

bool	is_builtin(char *name)
{
	if ((ft_strncmp(name, "echo", 4) == 0 && ft_strlen(name) == 4)
		|| (ft_strncmp(name, "pwd", 3) == 0 && ft_strlen(name) == 3)
		|| (ft_strncmp(name, "cd", 2) == 0 && ft_strlen(name) == 2)
		|| (ft_strncmp(name, "env", 6) == 0 && ft_strlen(name) == 3)
		|| (ft_strncmp(name, "export", 6) == 0 && ft_strlen(name) == 6)
		|| (ft_strncmp(name, "unset", 4) == 0 && ft_strlen(name) == 5)
		|| (ft_strncmp(name, "exit", 4) == 0 && ft_strlen(name) == 4))
		return (true);
	return (false);
}

bool	is_dot(char *cmd)
{
	if ((ft_strlen(cmd) == 1 && ft_strncmp(cmd, ".", 1) == 0)
		|| (ft_strlen(cmd) == 2 && ft_strncmp(cmd, "..", 2) == 0))
		return (true);
	return (false);
}
