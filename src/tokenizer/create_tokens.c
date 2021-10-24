#include "../../includes/minishell.h"

bool	last_token(int flag)
{
	if (flag == 1)
		return (true);
	return (false);
}

int	set_rd_tokens(char **tokens, char *cmd, int *i)
{
	if (cmd[*i + 1] == '>')
	{
		*tokens = ft_strdup(">>");
		(*i)++;
	}
	else if (cmd[*i] == '<')
		*tokens = ft_strdup("<");
	else if (cmd[*i] == '>')
		*tokens = ft_strdup(">");
	if (*tokens == NULL)
		return (FATAL_ERROR);
	(*i)++;
	while (cmd[*i] == ' ' || cmd[*i] == '\t')
		(*i)++;
	return (SUCCESS);
}

int	create_separator_token(t_vars *var, char **tokens, char *cmd)
{
	if (ft_strchr("<>", cmd[var->i]))
	{
		if (set_rd_tokens(&tokens[var->j], cmd, &var->i) == FATAL_ERROR)
			return (FATAL_ERROR);
		var->j++;
		var->start = var->i;
	}
	else
	{
		tokens[var->j] = ft_strdup("|");
		if (tokens[var->j] == NULL)
			return (FATAL_ERROR);
	}
	return (SUCCESS);
}

int	create_word_token(t_vars *var, char **tokens, char *cmd)
{
	if (cmd[var->i + 1] == '\0' && cmd[var->i] != '|')
		var->i++;
	tokens[var->j] = ft_substr(cmd, var->start, var->i - var->start);
	if (tokens[var->j] == NULL)
		return (FATAL_ERROR);
	if (cmd[var->i] != '\0' && !ft_strchr("<>|", cmd[var->i]))
	{
		var->i++;
		while (cmd[var->i] == ' ' || cmd[var->i] == '\t')
			var->i++;
		var->j++;
		var->start = var->i;
	}
	else
		var->j++;
	return (SUCCESS);
}

int	create_token(t_vars *var, char **tokens, char *cmd)
{
	if (cmd[var->i + 1] == '\0' || cmd[var->i] == '|')
		var->flag = 1;
	if (ft_strchr("<>|", cmd[var->i]))
	{
		if (var->i > 0)
		{
			if (cmd[var->i - 1] != ' ' && cmd[var->i - 1] != '\t')
			{
				if (create_word_token(var, tokens, cmd) == FATAL_ERROR)
					return (FATAL_ERROR);
			}
		}
		if (create_separator_token(var, tokens, cmd) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	else
	{
		if (create_word_token(var, tokens, cmd) == FATAL_ERROR)
			return (FATAL_ERROR);
	}
	return (SUCCESS);
}
