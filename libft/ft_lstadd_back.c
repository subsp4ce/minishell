#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		new->next = NULL;
	}
	else
	{
		last = ft_lstlast(*lst);
		last->next = new;
		new->next = NULL;
		new->prev = last;
	}
}
