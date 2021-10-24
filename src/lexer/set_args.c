#include "../../includes/minishell.h"

void	fill_arg(t_vars *var, t_cmd **new, char **tokens, int arg_count)
{
	if (!quote_state(&var->state, tokens[var->i][var->k]))
	{
		if (tokens[var->i][var->k] == '\\' && var->state.dq == ON)
		{
			if (tokens[var->i][var->k + 1] != '"'
				&& tokens[var->i][var->k + 1] != '`'
				&& tokens[var->i][var->k + 1] != '$'
				&& tokens[var->i][var->k + 1] != '\\')
			{
				(*new)->args[arg_count][var->j] = '\\';
				var->j++;
			}
			var->k++;
		}
		(*new)->args[arg_count][var->j] = tokens[var->i][var->k];
		var->j++;
	}
}

int	add_arg(t_vars *var, t_cmd **new, char **tokens, int arg_count)
{
	ft_bzero(&var->state, sizeof(var->state));
	(*new)->args[arg_count] = ft_calloc(var->count + 1, sizeof(char));
	if ((*new)->args[arg_count] == NULL)
		return (FATAL_ERROR);
	var->k = 0;
	while (var->j < var->count && tokens[var->i][var->k] != '\0')
	{
		fill_arg(var, new, tokens, arg_count);
		var->k++;
	}
	return (SUCCESS);
}

int	set_args(t_vars *var, t_cmd **new, char **tokens, int *arg_count)
{
	if (var->count == 0)
	{
		(*new)->args[*arg_count] = ft_strdup("");
		if ((*new)->args == NULL)
			return (FATAL_ERROR);
	}
	else if (add_arg(var, new, tokens, *arg_count) == FATAL_ERROR)
		return (FATAL_ERROR);
	(*arg_count)++;
	return (SUCCESS);
}
