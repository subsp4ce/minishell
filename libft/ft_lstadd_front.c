#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (new == NULL)
		return ;
	new->next = *lst;
	new->prev = NULL;
	(*lst)->prev = new;
	*lst = new;
}
