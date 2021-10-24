#include "../includes/minishell.h"

/*
**	var->flag checks if string is empty after expansion
*/

int	process_cmd(t_env *env, t_vars *var, char *input)
{
	t_cmd	*head;

	head = NULL;
	var->flag = OFF;
	if (parser(env, &head, var, input) == FATAL_ERROR)
		return (FATAL_ERROR);
	if (var->flag == ON && input[var->i] == '\0')
		return (SUCCESS);
	if (var->flag == OFF)
	{
		if (executer(&head, env) == FATAL_ERROR)
		{
			free_tokens(&head);
			return (FATAL_ERROR);
		}
	}
	free_tokens(&head);
	return (SUCCESS);
}

bool	cmd_separator(t_quote *state, char *input, int i)
{
	if (input[i] == '\0'
		|| (input[i] == ';' && is_separator(state))
		|| input[i + 1] == '\0')
		return (true);
	return (false);
}

int	process_input(t_env *env, char *input)
{
	t_vars	var;

	ft_bzero(&var, sizeof(t_vars));
	while (input[var.i])
	{
		quote_state(&var.state, input[var.i]);
		if (cmd_separator(&var.state, input, var.i))
		{
			if (process_cmd(env, &var, input) == FATAL_ERROR)
				return (FATAL_ERROR);
		}
		if (input[var.i] == '|' && is_separator(&var.state))
			var.pipe++;
		if (var.state.dq == ON && input[var.i] == '\\')
			var.i += 2;
		else if (input[var.i] != '\0')
			var.i++;
	}
	return (SUCCESS);
}
