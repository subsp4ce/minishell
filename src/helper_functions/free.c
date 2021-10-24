#include "../../includes/minishell.h"

void	free_str(char **str)
{
	if (*str != NULL)
	{
		free(*str);
		*str = NULL;
	}
}

int	free_and_ret(char **str, int ret)
{
	if (*str != NULL)
	{
		free(*str);
		*str = NULL;
	}
	return (ret);
}

char	**free_2d_array(char ***array)
{
	int	i;

	i = 0;
	if (*array != NULL)
	{
		while ((*array)[i])
		{
			free((*array)[i]);
			(*array)[i] = NULL;
			i++;
		}
		free(*array);
		*array = NULL;
	}
	return (NULL);
}

void	free_rd(t_cmd **head)
{
	t_rd	*ptr;

	ptr = NULL;
	while ((*head)->rd_head != NULL)
	{
		free((*head)->rd_head->fn);
		(*head)->rd_head->fn = NULL;
		ptr = (*head)->rd_head;
		(*head)->rd_head = (*head)->rd_head->next;
		free(ptr);
		ptr = NULL;
	}
}

t_cmd	*free_tokens(t_cmd **head)
{
	t_cmd	*tmp;

	tmp = NULL;
	while (*head != NULL)
	{
		if ((*head)->name != NULL)
		{
			free((*head)->name);
			(*head)->name = NULL;
		}
		if ((*head)->args != NULL)
			free_2d_array(&(*head)->args);
		if ((*head)->argv != NULL)
			free_2d_array(&(*head)->argv);
		if ((*head)->rd_head != NULL)
			free_rd(head);
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
	}
	free(*head);
	*head = NULL;
	return (NULL);
}
