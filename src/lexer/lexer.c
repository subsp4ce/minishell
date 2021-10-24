#include "../../includes/minishell.h"

/* 
**	Count number of printable characters for one token
*/

void	get_char_count(t_vars *var, char **tokens)
{
	var->j = 0;
	var->count = 0;
	while (tokens[var->i][var->j])
	{
		if (!quote_state(&var->state, tokens[var->i][var->j]))
		{
			if (tokens[var->i][var->j] == '\\' && var->state.dq == ON)
			{
				if (is_escape(tokens[var->i][var->j + 1]))
				{
					var->count++;
					var->j++;
				}
				else
					var->count++;
			}
			else
				var->count++;
		}
		var->j++;
	}
	var->j = 0;
}

/*
**	Deduct rd and fn tokens from total token count
*/

int	arg_counter(char **tokens)
{
	int		i;
	int		rd_fn;

	i = 0;
	rd_fn = 0;
	if (tokens != NULL)
	{
		while (tokens[i])
		{
			if (ft_strchr("<>", tokens[i][0]) && tokens[i][0] != '\0')
				rd_fn += 2;
			if (tokens[i][0] == '|')
				break ;
			i++;
		}
	}
	return (i - rd_fn);
}

void	add_node(t_cmd **head, t_cmd *new)
{
	t_cmd	*ptr;

	ptr = *head;
	if (*head == NULL)
		*head = new;
	else
	{
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
	}
}

/*
**	Subtract one from count for command
*/

t_cmd	*create_node(t_cmd **head, char **tokens)
{
	t_cmd	*new;
	int		arg_count;

	new = ft_calloc(1, sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	ft_bzero(new, sizeof(t_cmd));
	add_node(head, new);
	arg_count = arg_counter(tokens) - 1;
	if (arg_count > 0)
	{
		new->args = ft_calloc(arg_count + 1, sizeof(char *));
		if (new->args == NULL)
		{
			free(new);
			return (NULL);
		}
	}
	return (new);
}

t_cmd	*lexer(t_cmd **head, char **tokens)
{
	t_vars	var;
	t_cmd	*new;
	int		arg_count;

	ft_bzero(&var, sizeof(t_vars));
	new = create_node(head, tokens);
	if (new == NULL)
		return (NULL);
	arg_count = 0;
	while (tokens[var.i])
	{
		ft_bzero(&var.state, sizeof(var.state));
		get_char_count(&var, tokens);
		if (set_token_id(&var, &new, tokens, &arg_count) == FATAL_ERROR)
			return (free_tokens(head));
		if (new->operator == '|')
			break ;
		var.i++;
	}
	if (new->name != NULL)
	{
		if (set_expression(&var, &new, arg_count) == FATAL_ERROR)
			return (free_tokens(head));
	}
	return (*head);
}
