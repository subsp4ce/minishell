#include "../includes/minishell.h"

char	**free_var(char **var)
{
	free(*var);
	*var = NULL;
	return (NULL);
}

bool	end_of_input(char *line, int i)
{
	if (line[i] != '\0')
	{
		if (line[i + 1] == '\0' && line[i] != ';')
			return (true);
	}
	return (false);
}

char	**get_cmd(t_env *env, char *line, t_vars *var)
{
	char	*str;
	char	*cmd_exp;

	cmd_exp = NULL;
	if (end_of_input(line, var->i))
		(var->i)++;
	str = ft_substr(line, var->start, var->i - var->start);
	if (str == NULL)
		return (NULL);
	if (has_expansion(str))
	{
		cmd_exp = expander(env, str, &var->flag, var->pipe);
		if ((cmd_exp == NULL && var->flag == OFF) || (var->flag == ON))
			return (free_var(&str));
		free(str);
		str = ft_strdup(cmd_exp);
		if (str == NULL)
			return (free_var(&cmd_exp));
		free(cmd_exp);
	}
	return (get_cmd_2d_array(&str, var->pipe));
}

/*
**	Scenarios:
**	- cmd remains unchanged (no expansions)
**	- cmd is empty string after expansion: "$doesntexist"
**	- cmd is expanded and/or expansions that don't exist are removed
*/

int	parser(t_env *env, t_cmd **head, t_vars *var, char *line)
{
	char	**cmd;

	cmd = get_cmd(env, line, var);
	if (cmd == NULL && var->flag == OFF)
		return (FATAL_ERROR);
	var->start = var->i + 1;
	var->pipe = 0;
	if (var->flag == ON)
		return (SUCCESS);
	if (tokenizer(head, cmd) == FATAL_ERROR)
	{
		free_2d_array(&cmd);
		return (FATAL_ERROR);
	}
	free_2d_array(&cmd);
	return (SUCCESS);
}
