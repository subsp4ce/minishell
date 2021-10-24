#include "../../includes/minishell.h"

bool	is_expansion(t_quote *state, char *token, int i)
{
	if (token[i + 1] == ' ' || token[i + 1] == '\t')
		return (false);
	if (i > 0)
	{
		if (token[i - 1] == '\\')
		{
			if (i > 1)
			{
				if (token[i - 2] == '\\')
					return (true);
			}
			return (false);
		}
	}
	if (state->sq == OFF)
		return (true);
	return (false);
}

bool	is_home_expansion(char *token, int i)
{
	if ((ft_strlen(token) == 1)
		|| (i == 0 && token[i + 1] == ' ')
		|| (i == 0 && token[i + 1] == '\t')
		|| ((i > 0 && token[i - 1] == ' ') && (token[i + 1] == ' '))
		|| ((i > 0 && token[i - 1] == '\t') && (token[i + 1] == '\t'))
		|| ((i > 0 && token[i - 1] == ' ') && (token[i + 1] == '\t'))
		|| ((i > 0 && token[i - 1] == '\t') && (token[i + 1] == ' '))
		|| ((i > 0 && token[i + 1] == '\0') && (token[i - 1] == ' '))
		|| ((i > 0 && token[i + 1] == '\0') && (token[i - 1] == '\t'))
		|| (i == 0 && token[i + 1] == '/')
		|| ((i > 0 && token[i - 1] == ' ') && (token[i + 1] == '/'))
		|| ((i > 0 && token[i - 1] == '\t') && (token[i + 1] == '/')))
		return (true);
	return (false);
}

bool	has_expansion(char *token)
{
	t_quote	state;
	int		i;

	ft_bzero(&state, sizeof(t_quote));
	i = 0;
	while (token[i])
	{
		quote_state(&state, token[i]);
		if (token[i] == '$' && is_expansion(&state, token, i))
			return (true);
		else if (token[i] == '~' && is_home_expansion(token, i))
			return (true);
		i++;
	}
	return (false);
}
