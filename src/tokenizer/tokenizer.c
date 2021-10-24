#include "../../includes/minishell.h"

int	free_tokenizer(char ***tokens, char **str)
{
	free_2d_array(tokens);
	if (str != NULL)
	{
		free(*str);
		str = NULL;
	}
	return (FATAL_ERROR);
}

char	**get_tokens(char *cmd, int count)
{
	t_vars	var;
	char	**tokens;

	ft_bzero(&var, sizeof(t_vars));
	tokens = ft_calloc(count + 1, sizeof(char *));
	if (tokens == NULL)
		return (NULL);
	while (cmd[var.i])
	{
		quote_state(&var.state, cmd[var.i]);
		if (var.state.dq == ON && cmd[var.i] == '\\'
			&& cmd[var.i + 1] != '\0')
			var.i += 1;
		if ((ft_strchr(" <>|\t", cmd[var.i]) && is_separator(&var.state))
			|| cmd[var.i + 1] == '\0')
		{
			if (create_token(&var, tokens, cmd) == FATAL_ERROR)
				return (free_2d_array(&tokens));
			if (last_token(var.flag))
				return (tokens);
		}
		else
			var.i++;
	}
	return (tokens);
}

void	token_found(t_vars *var, char *cmd)
{
	if (cmd[var->i] == ' ' || cmd[var->i] == '\t')
	{
		while (cmd[var->i] == ' ' || cmd[var->i] == '\t')
			var->i++;
		if (!ft_strchr("|<>\0", cmd[var->i]))
			var->count++;
	}
	else if (ft_strchr("<>|", cmd[var->i]))
	{
		var->count++;
		var->i++;
		if (ft_strchr("<>", cmd[var->i - 1]))
		{
			if (cmd[var->i] == '>')
				var->i++;
			if (!ft_strchr(" \0\t", cmd[var->i]))
			{
				var->count++;
				var->i++;
			}
		}
	}
}

int	token_count(char *cmd)
{
	t_vars	var;

	ft_bzero(&var, sizeof(t_vars));
	var.count = 1;
	while (cmd[var.i])
	{
		if (var.state.sq == OFF && cmd[var.i] == '\\'
			&& cmd[var.i + 1] != '\0')
			var.i += 1;
		quote_state(&var.state, cmd[var.i]);
		if (ft_strchr(" |<>\t", cmd[var.i]) && is_separator(&var.state))
			token_found(&var, cmd);
		else
			var.i++;
	}
	return (var.count);
}

int	tokenizer(t_cmd **head, char **cmd)
{
	t_vars	var;
	char	**tokens;
	char	*str;

	ft_bzero(&var, sizeof(t_vars));
	while (cmd[var.i])
	{
		str = trim_spaces(cmd[var.i]);
		if (str == NULL)
			return (FATAL_ERROR);
		var.count = token_count(str);
		tokens = get_tokens(str, var.count);
		if (tokens == NULL)
			return (free_tokenizer(&tokens, &str));
		*head = lexer(head, tokens);
		if (*head == NULL)
			return (free_tokenizer(&tokens, &str));
		var.i++;
		free_tokenizer(&tokens, &str);
	}
	return (SUCCESS);
}
