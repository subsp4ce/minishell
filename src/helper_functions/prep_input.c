#include "../../includes/minishell.h"

void	trim_front_back(char *line, int *len, int *start, int *end)
{
	while (is_whitespace(line[*start]))
	{
		(*start)++;
		(*len)--;
	}
	while (is_whitespace(line[*end]))
	{
		(*end)--;
		(*len)--;
	}
}

char	*trim_spaces(char *line)
{
	int		start;
	int		end;
	int		len;

	start = 0;
	len = ft_strlen(line);
	end = ft_strlen(line) - 1;
	if (len == 0)
		return (ft_strdup(""));
	trim_front_back(line, &len, &start, &end);
	return (ft_substr(line, start, len));
}

int	prep_input(t_env *env, char *line)
{
	char	*input;

	input = trim_spaces(line);
	if (input == NULL)
		return (FATAL_ERROR);
	if (scanner(input) == SYNTAX_ERROR)
		return (free_and_ret(&input, SYNTAX_ERROR));
	if (process_input(env, input) == FATAL_ERROR)
		return (free_and_ret(&input, FATAL_ERROR));
	free_str(&input);
	return (SUCCESS);
}
