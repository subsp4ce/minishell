#include "../../includes/minishell.h"

int	is_redirection(char *line, int i)
{
	if (line[i] == '>' && line[i + 1] == '>')
		return (DBL_OUT);
	if (line[i] == '<' && line[i + 1] == '<')
		return (DBL_IN);
	if (line[i] == '<' && line[i + 1] == '>')
		return (IN_OUT);
	if (line[i] == '>')
		return (OUT);
	if (line[i] == '<')
		return (IN);
	return (SUCCESS);
}

int	check_redirection(char *line, int *i)
{
	int		rd[2];
	int		j;

	if (line[*i] == '<' || line[*i] == '>')
	{
		rd[0] = is_redirection(line, *i);
		(*i)++;
		if (rd[0] == DBL_OUT || rd[0] == DBL_IN || rd[0] == IN_OUT)
			(*i)++;
		j = *i;
		while (line[j] == ' ' || line[j] == '\t')
			j++;
		if (line[j] == '\0' || line[j] == ';')
			return (syntax_error('\n'));
		rd[1] = is_redirection(line, j);
		if (check_rd_error(rd) == SYNTAX_ERROR)
			return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

int	check_separator(char *line, int *i)
{
	if (line[*i] == ';' || line[*i] == '|' || line[*i] == '\\')
	{
		if (ft_strchr(";|", line[*i]) && (ft_strlen(line) == 1 || *i == 0))
			return (syntax_error(line[*i]));
		if (line[*i] == '\\')
			return (syntax_error(line[*i]));
		while (line[*i + 1] == ' ' || line[*i + 1] == '\t')
			(*i)++;
		if (line[*i] == '|' && line[*i + 1] == '\0')
			return (syntax_error(line[*i + 1]));
		if (line[*i + 1] == '|' || line[*i + 1] == ';')
			return (syntax_error(line[*i + 1]));
	}
	return (SUCCESS);
}

int	check_line(t_quote *state, char *line, int *i)
{
	quote_state(state, line[*i]);
	if (line[*i] == '\\' && state->dq == ON)
		(*i)++;
	else if (ft_strchr("<>;|\\", line[*i]) && is_separator(state))
	{
		if (check_separator(line, i) == SYNTAX_ERROR
			|| check_redirection(line, i) == SYNTAX_ERROR)
			return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

int	scanner(char *line)
{
	t_quote	state;
	int		i;

	ft_bzero(&state, sizeof(t_quote));
	i = 0;
	while (line[i])
	{
		if (check_line(&state, line, &i) == SYNTAX_ERROR)
			return (SYNTAX_ERROR);
		if (line[i] != '\0')
			i++;
	}
	if (state.dq == ON || state.sq == ON)
	{
		error_msg("multiline commands not supported", "");
		g_exit_status = 1;
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
