#include "../../includes/minishell.h"

int	set_relative_path(t_cmd *head)
{
	t_vars	var;

	ft_bzero(&var, sizeof(t_vars));
	var.i = 5;
	var.len = ft_strlen(head->name);
	head->argv[0] = ft_calloc(var.len - 4, sizeof(char));
	if (head->argv[0] == NULL)
		return (FATAL_ERROR);
	while (head->name[var.i] != '\0')
	{
		head->argv[0][var.j] = head->name[var.i];
		var.j++;
		var.i++;
	}
	head->argv[0][var.j] = '\0';
	return (SUCCESS);
}

int	set_expression(t_vars *var, t_cmd **new, int arg_count)
{
	var->i = 0;
	var->count = arg_count + 1;
	(*new)->argv = ft_calloc(var->count + 1, sizeof(char *));
	if ((*new)->argv == NULL)
		return (FATAL_ERROR);
	if (ft_strncmp((*new)->name, "/bin/", 5) == 0)
		set_relative_path(*new);
	else
		(*new)->argv[0] = ft_strdup((*new)->name);
	if ((*new)->argv[0] == NULL)
		return (FATAL_ERROR);
	var->i = 0;
	if ((*new)->args != NULL)
	{
		while ((*new)->args[var->i] != NULL)
		{
			(*new)->argv[var->i + 1] = ft_strdup((*new)->args[var->i]);
			if ((*new)->argv[var->i + 1] == NULL)
				return (FATAL_ERROR);
			var->i++;
		}
	}
	return (SUCCESS);
}

void	fill_command(t_vars *var, t_cmd **new, char **tokens)
{
	if (tokens[var->i][var->k] == '\\' && var->state.dq == ON)
	{
		if (is_escape(tokens[var->i][var->k + 1]))
			var->k++;
		(*new)->name[var->j] = tokens[var->i][var->k];
		var->k++;
		var->j++;
	}
	else
	{
		(*new)->name[var->j] = tokens[var->i][var->k];
		var->k++;
		var->j++;
	}
}

int	set_command(t_vars *var, t_cmd **new, char **tokens)
{
	if (var->count == 0)
		(*new)->name = ft_strdup("");
	else
		(*new)->name = ft_calloc(var->count + 1, sizeof(char));
	if ((*new)->name == NULL)
		return (FATAL_ERROR);
	if (var->count > 0)
	{		
		var->k = 0;
		var->j = 0;
		while (var->j < var->count)
		{
			if (!quote_state(&var->state, tokens[var->i][var->k]))
				fill_command(var, new, tokens);
			else
				var->k++;
		}
		(*new)->name[var->j] = '\0';
	}
	return (SUCCESS);
}

int	set_token_id(t_vars *var, t_cmd **new, char **tokens, int *arg_count)
{
	int		res;

	res = 0;
	if (tokens[var->i][0] == '|')
	{
		(*new)->operator = '|';
		return (SUCCESS);
	}
	else if (fn_token(var->flag))
		res = set_fn(var, new, tokens);
	else if (ft_strchr("<>", tokens[var->i][var->j])
		&& tokens[var->i][var->j] != '\0')
		res = set_rd(var, new, tokens);
	else if ((*new)->name == NULL)
		res = set_command(var, new, tokens);
	else
		res = set_args(var, new, tokens, arg_count);
	if (res == FATAL_ERROR)
		return (FATAL_ERROR);
	return (SUCCESS);
}
