#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dst == NULL && dstsize == 0)
		return (ft_strlen(src));
	while (dst[i] && dstsize > i)
		i++;
	if (i + 1 < dstsize)
		ft_strlcpy(&dst[i], src, dstsize - i);
	return (i + ft_strlen(src));
}
