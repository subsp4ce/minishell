#include "../../includes/minishell.h"

/* 
	get_cmd returns a double array of commands separated by pipes
	if no pipes a single command is returned
*/

char	*get_pipe_cmd(t_vars *var, char *cmd)
{
	char	*pipe_cmd;

	pipe_cmd = ft_substr(cmd, var->start, var->i - var->start + 1);
	if (pipe_cmd == NULL)
		return (NULL);
	var->start = var->i + 1;
	while ((cmd[var->start] == ' ' || cmd[var->start] == '\t')
		&& is_separator(&var->state))
		var->start++;
	return (pipe_cmd);
}

char	**pipe_split(char *cmd, int pipe)
{
	t_vars	var;
	char	**pipe_cmds;

	ft_bzero(&var, sizeof(t_vars));
	pipe_cmds = ft_calloc(pipe + 2, sizeof(char *));
	if (pipe_cmds == NULL)
		return (NULL);
	while (cmd[var.i])
	{
		quote_state(&var.state, cmd[var.i]);
		if ((cmd[var.i] == '|' && is_separator(&var.state))
			|| cmd[var.i + 1] == '\0')
		{
			pipe_cmds[var.j] = get_pipe_cmd(&var, cmd);
			if (pipe_cmds[var.j] == NULL)
				return (free_2d_array(&pipe_cmds));
			var.j++;
		}
		if (var.state.dq == ON && cmd[var.i] == '\\')
			var.i += 2;
		else if (cmd[var.i] != '\0')
			var.i++;
	}
	return (pipe_cmds);
}

char	**get_single_cmd(char *str)
{
	char	**cmd;

	cmd = ft_calloc(2, sizeof(char *));
	if (cmd == NULL)
		return (NULL);
	cmd[0] = ft_strdup(str);
	if (cmd[0] == NULL)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

char	**get_cmd_2d_array(char **str, int pipe)
{
	char	**cmd;

	cmd = NULL;
	if (pipe == 0)
		cmd = get_single_cmd(*str);
	else
		cmd = pipe_split(*str, pipe);
	if (cmd == NULL)
	{
		free(*str);
		return (NULL);
	}
	free(*str);
	return (cmd);
}
