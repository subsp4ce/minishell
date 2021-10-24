#include "libft.h"

int	ft_atoi(const char *str)
{
	int	n;
	int	flag;
	int	i;

	n = 0;
	flag = 1;
	i = 0;
	while (str[i] == 32 || (str[i] > 8 && str[i] < 14)
		|| (str[i] == '+' && str[i + 1] >= '0' && str[i + 1] <= '9'))
		i++;
	if (str[i] == '-')
	{
		flag = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = (n * 10) + (str[i] - '0');
		i++;
	}
	return (n * flag);
}
