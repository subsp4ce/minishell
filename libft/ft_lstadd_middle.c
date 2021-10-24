#include "libft.h"

void	ft_lstadd_middle(t_list *lst, t_list *new)
{
	new->prev = lst->prev;
	new->next = lst;
	lst->prev->next = new;
	lst->prev = new;
}
