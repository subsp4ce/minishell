#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_list	*delnode;

	tmp = *lst;
	delnode = *lst;
	if (lst == NULL || del == NULL)
		return ;
	while (tmp != NULL)
	{
		del(tmp->content);
		delnode = tmp;
		tmp = tmp->next;
		free(delnode);
	}
	*lst = NULL;
}
