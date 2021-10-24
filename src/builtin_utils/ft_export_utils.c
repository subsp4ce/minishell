#include "../../includes/minishell.h"

int	ft_strncmp_alnum(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && (unsigned char)s1[i] == (unsigned char)s2[i]
		&& (i + 1 < n))
		i++;
	if (ft_isdigit(s1[i]) && !ft_isdigit(s2[i]))
		return (1);
	if (ft_isdigit(s2[i]) && !ft_isdigit(s1[i]))
		return (-1);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	sort_export_vars(t_list **head, t_list *new)
{
	t_list	*ptr;

	ptr = *head;
	if (*head == NULL)
		*head = new;
	else if (ft_strncmp_alnum((*head)->content, new->content,
			ft_strlen((*head)->content)) > 0)
		return (ft_lstadd_front(head, new));
	while (ptr != NULL)
	{
		if (ft_strncmp_alnum(ptr->content, new->content,
				ft_strlen(ptr->content)) > 0)
			return (ft_lstadd_middle(ptr, new));
		ptr = ptr->next;
		if (ptr == NULL)
			return (ft_lstadd_back(head, new));
	}
}

char	*insert_double_quotes(char *str)
{
	int		i;
	int		j;
	int		flag;
	char	*var;

	i = 0;
	j = 0;
	flag = 0;
	var = ft_calloc(ft_strlen(str) + 3, sizeof(char));
	if (var == NULL)
		return (NULL);
	while (str[i])
	{
		var[j] = str[i];
		if (str[i] == '=' && flag == 0)
		{
			flag = 1;
			j++;
			var[j] = '"';
		}
		i++;
		j++;
	}
	var[j] = '"';
	return (var);
}
