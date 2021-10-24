#include "../includes/minishell.h"

bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

bool	is_separator(t_quote *state)
{
	if (state->dq == OFF && state->sq == OFF)
		return (true);
	return (false);
}

bool	quote_state(t_quote *state, char c)
{
	if (c == '"' && state->dq == ON)
		state->dq = OFF;
	else if (c == '\'' && state->sq == ON)
		state->sq = OFF;
	else if (c == '"' && state->sq == OFF)
		state->dq = ON;
	else if (c == '\'' && state->dq == OFF)
		state->sq = ON;
	else
		return (false);
	return (true);
}

bool	empty_pipe(char *str, int pipe_count)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|')
			pipe++;
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '|')
			return (false);
		i++;
	}
	if (pipe == pipe_count)
		return (true);
	return (false);
}

bool	empty_line(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	while (line[i] == ' ' || line[i] == '\t')
	{
		i++;
		len--;
	}
	if (len == 0)
		return (true);
	return (false);
}
