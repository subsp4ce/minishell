#include "../../includes/minishell.h"

char	*get_var_name(char *cmd, int len, int i)
{
	char	*name;
	int		j;

	if (cmd[i] == '~')
		return (ft_strdup("HOME="));
	else
		name = ft_calloc(len + 2, sizeof(char));
	if (name == NULL)
		return (NULL);
	j = 0;
	if (cmd[i] == '$')
		i++;
	while (cmd[i] && len > 0)
	{
		name[j] = cmd[i];
		i++;
		j++;
		len--;
	}
	name[j] = '=';
	j++;
	name[j] = '\0';
	return (name);
}

int	get_param_len(char *token, int i)
{
	int	len;

	len = 0;
	if (token[i] == '~')
		return (SUCCESS);
	if (token[i] == '$')
		i++;
	while (token[i] && ft_strchr("\"\'$/-*#= \t:", token[i]) == 0)
	{
		len++;
		i++;
	}
	return (len);
}

char	*expand_variable(t_env *env, t_vars *var, char *cmd)
{
	char	*value;
	char	*name;

	value = NULL;
	name = NULL;
	if (!ft_isalpha(cmd[var->i + 1])
		&& cmd[var->i] != '~' && cmd[var->i + 1] != '_')
		return (expand_special_param(var, cmd));
	var->len = get_param_len(cmd, var->i);
	name = get_var_name(cmd, var->len, var->i);
	if (name == NULL)
		return (NULL);
	value = extract_value(env, name, cmd[var->i]);
	if (value == NULL)
	{
		free(name);
		return (value);
	}
	free(name);
	var->i = var->i + var->len;
	return (value);
}
