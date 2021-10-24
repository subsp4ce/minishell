#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;

	i = 0;
	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(s1));
	len = ft_strlen(s1);
	while (s1[i] && ft_strchr(set, s1[i]) != NULL)
		i++;
	while (len > 0 && ft_strchr(set, s1[len]) != NULL)
		len--;
	if (len <= 0)
		return (ft_strdup(""));
	return (ft_substr(s1, i, len - i + 1));
}
