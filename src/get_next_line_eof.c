#include "../includes/minishell.h"

static int	read_line(char **str, char *buff, char **line)
{
	int	i;
	int	y;

	i = 0;
	y = 0;
	while ((*str)[i] != '\n' && (*str)[i] != '\0')
		i++;
	*line = ft_substr(*str, 0, i);
	if (*line == NULL)
		return (free_and_ret(str, FATAL_ERROR));
	while (buff[y] != '\n' && buff[y] != '\0')
		y++;
	if (buff[y] == '\n')
		ft_memmove(buff, buff + y + 1, ft_strlen(buff) - y);
	free_str(str);
	return (SUCCESS);
}

static int	process_read(char *buff, char **str, int ret, int chars_read)
{
	char	*tmp;

	tmp = NULL;
	write(STDOUT_FILENO, "  \b\b", 4);
	if (ret == 0 && ft_strlen(buff) == 0 && chars_read == 0)
	{
		free_str(str);
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(0);
	}
	buff[ret] = '\0';
	tmp = ft_strjoin(*str, buff);
	if (tmp == NULL)
		return (free_and_ret(str, FATAL_ERROR));
	free_str(str);
	*str = ft_strdup(tmp);
	if (*str == NULL)
		return (free_and_ret(&tmp, FATAL_ERROR));
	free(tmp);
	return (SUCCESS);
}

void	check_ctrl_d(ssize_t *ret, char *str, int *chars_read)
{
	if (*ret == 0 && ft_strlen(str) > 0)
	{
		*chars_read = 1;
		*ret = 1;
	}
}

int	get_next_line_eof(int fd, char **line)
{
	static char	buff[1][BUFFER_SIZE + 1];
	char		*str;
	ssize_t		ret;
	int			chars_read;

	ret = 1;
	str = NULL;
	chars_read = 0;
	if (fd < 0 || BUFFER_SIZE == 0)
		return (FATAL_ERROR);
	str = ft_strdup(buff[fd]);
	if (str == NULL)
		return (FATAL_ERROR);
	while (ret > 0 && ft_strchr(buff[fd], '\n') == NULL)
	{
		ret = read(fd, buff[fd], BUFFER_SIZE);
		if (ret == ERROR)
			return (free_and_ret(&str, FATAL_ERROR));
		if (process_read(buff[fd], &str, ret, chars_read) == FATAL_ERROR)
			return (FATAL_ERROR);
		check_ctrl_d(&ret, str, &chars_read);
	}
	return (read_line(&str, buff[fd], line));
}
