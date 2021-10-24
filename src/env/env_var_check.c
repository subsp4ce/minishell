#include "../../includes/minishell.h"

bool	var_exist(t_list **head, char *id)
{
	t_list	*lst;
	int		len;
	int		i;

	lst = *head;
	len = ft_strlen(id);
	while (lst != NULL)
	{
		i = 0;
		while (lst->content[i] != '=' && lst->content[i] != '\0')
			i++;
		if (ft_strncmp(lst->content, id, len) == 0 && i == len)
			return (true);
		lst = lst->next;
	}
	return (false);
}
