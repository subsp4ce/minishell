#include "../../includes/minishell.h"

t_rd	*find_node(t_rd **head)
{
	t_rd	*ptr;

	ptr = *head;
	while (ptr->next != NULL)
		ptr = ptr->next;
	return (ptr);
}

int	set_fn(t_vars *var, t_cmd **new, char **tokens)
{
	t_rd	*new_rd;

	new_rd = find_node(&(*new)->rd_head);
	new_rd->fn = ft_calloc(var->count + 1, sizeof(char));
	if (new_rd->fn == NULL)
		return (FATAL_ERROR);
	var->k = 0;
	var->j = 0;
	while (var->j < var->count)
	{
		if (!quote_state(&var->state, tokens[var->i][var->k]))
		{
			new_rd->fn[var->j] = tokens[var->i][var->k];
			var->j++;
			var->k++;
		}
		else
			var->k++;
	}
	new_rd->fn[var->j] = '\0';
	var->flag = 0;
	return (SUCCESS);
}

void	add_rd_node(t_rd **head, t_rd *new)
{
	t_rd	*ptr;

	ptr = *head;
	if (*head == NULL)
		*head = new;
	else
	{
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
	}
}

int	set_rd(t_vars *var, t_cmd **new, char **tokens)
{
	t_rd	*rd_new;

	rd_new = ft_calloc(1, sizeof(t_rd));
	if (rd_new == NULL)
		return (FATAL_ERROR);
	ft_bzero(rd_new, sizeof(t_rd));
	add_rd_node(&(*new)->rd_head, rd_new);
	if (tokens[var->i][var->j] == '>' && tokens[var->i][var->j + 1] == '>')
		rd_new->type = DBL_OUT;
	else if (tokens[var->i][var->j] == '>')
		rd_new->type = OUT;
	else
		rd_new->type = IN;
	var->flag = ON;
	return (SUCCESS);
}
