int	ft_isascii(int c)
{
	if (c >= 000 && c <= 0177)
		return (1);
	return (0);
}
