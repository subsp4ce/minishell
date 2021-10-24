#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned const char	*s;
	unsigned char		*d;
	size_t				i;

	s = src;
	d = dst;
	i = 0;
	if (len == 0 || d == s)
		return (d);
	while (d <= s && len > i)
	{
		d[i] = s[i];
		i++;
	}
	while (d > s && len > 0)
	{
		d[len - 1] = s[len - 1];
		len--;
	}
	return (dst);
}
