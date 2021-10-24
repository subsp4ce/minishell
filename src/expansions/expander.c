#include "../../includes/minishell.h"

char	*free_expansion(char **value, char **cmd_expansion)
{
	if (*value != NULL)
	{
		free(*value);
		*value = NULL;
	}
	if (*cmd_expansion != NULL)
	{
		free(*cmd_expansion);
		*cmd_expansion = NULL;
	}
	return (NULL);
}

int	concatenate_expansion(char *value, char **cmd_exp)
{
	char	*tmp;

	tmp = NULL;
	if (*cmd_exp == NULL)
	{
		*cmd_exp = ft_strdup(value);
		if (*cmd_exp == NULL)
			return (FATAL_ERROR);
		return (SUCCESS);
	}
	tmp = ft_strdup(*cmd_exp);
	if (tmp == NULL)
		return (FATAL_ERROR);
	free(*cmd_exp);
	*cmd_exp = ft_strjoin(tmp, value);
	if (*cmd_exp == NULL)
	{
		free(tmp);
		return (FATAL_ERROR);
	}
	free(tmp);
	return (SUCCESS);
}

char	*get_value(t_env *env, t_vars *var, char *cmd)
{
	char	*value;

	value = NULL;
	if ((cmd[var->i] == '$' && is_expansion(&var->state, cmd, var->i))
		|| (cmd[var->i] == '~' && is_home_expansion(cmd, var->i)))
	{
		value = expand_variable(env, var, cmd);
		if (value == NULL)
			return (NULL);
	}
	else
	{
		var->len = get_len_substr(cmd, var->i, var);
		value = ft_substr(cmd, var->i, var->len);
		if (value == NULL)
			return (NULL);
		var->i = var->i + var->len - 1;
	}
	return (value);
}

char	*get_expansion(t_env *env, char *cmd, int *empty_flag)
{
	t_vars	var;
	char	*value;
	char	*cmd_expansion;

	ft_bzero(&var, sizeof(t_vars));
	value = NULL;
	cmd_expansion = NULL;
	while (cmd[var.i])
	{
		value = get_value(env, &var, cmd);
		if (value == NULL)
			return (free_expansion(&value, &cmd_expansion));
		if (concatenate_expansion(value, &cmd_expansion) == FATAL_ERROR)
			return (free_expansion(&value, &cmd_expansion));
		free_str(&value);
		var.i++;
	}
	if (cmd_expansion == NULL)
		*empty_flag = ON;
	return (cmd_expansion);
}

char	*expander(t_env *env, char *cmd, int *empty_flag, int pipe)
{
	char	*cmd_trim_spaces;
	char	*cmd_exp;

	cmd_trim_spaces = trim_spaces(cmd);
	if (cmd_trim_spaces == NULL)
		return (NULL);
	cmd_exp = get_expansion(env, cmd_trim_spaces, empty_flag);
	if (cmd_exp == NULL && *empty_flag == OFF)
	{
		free(cmd_trim_spaces);
		return (NULL);
	}
	if (empty_line(cmd_exp) || empty_pipe(cmd_exp, pipe))
	{
		*empty_flag = ON;
		free(cmd_exp);
		cmd_exp = NULL;
	}
	free(cmd_trim_spaces);
	return (cmd_exp);
}
