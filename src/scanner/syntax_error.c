#include "../../includes/minishell.h"

int	syntax_error(char c)
{
	if (c == '\0')
	{
		error_msg("multiline commands not supported", "");
		g_exit_status = 1;
		return (SYNTAX_ERROR);
	}
	else if (c == '\\')
		error_msg("syntax error near unexpected token `\\'", "");
	else if (c == ';')
		error_msg("syntax error near unexpected token `;'", "");
	else if (c == '|')
		error_msg("syntax error near unexpected token `|'", "");
	else if (c == '\n')
		error_msg("syntax error near unexpected token `newline'",
			"");
	g_exit_status = 258;
	return (SYNTAX_ERROR);
}

int	unsupported_rd(int *rd)
{
	if (rd[0] == DBL_IN || rd[0] == IN_OUT)
	{
		if (rd[0] == DBL_IN)
			ft_putstr_fd("minishell: `<<' is not supported\n", 2);
		if (rd[0] == IN_OUT)
			ft_putstr_fd("minishell: `<>' is not supported\n", 2);
		g_exit_status = 1;
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

int	check_rd_error(int *rd)
{
	if (rd[0] != 0 && rd[1] != 0)
	{
		if (rd[0] == DBL_IN || rd[1] == DBL_IN)
		{
			ft_putstr_fd("minishell: `<<' is not supported\n", 1);
			g_exit_status = 1;
			return (SYNTAX_ERROR);
		}
		else if (rd[1] == DBL_OUT)
			error_msg("syntax error near unexpected token `>>'", "");
		else if (rd[1] == OUT)
			error_msg("syntax error near unexpected token `>'", "");
		else if (rd[1] == IN)
			error_msg("syntax error near unexpected token `<'", "");
		else if (rd[1] == IN_OUT)
			error_msg("syntax error near unexpected token `<>'", "");
		else
			error_msg("syntax error near unexpected token `newline'", "");
		g_exit_status = 258;
		return (SYNTAX_ERROR);
	}
	if (unsupported_rd(rd) == SYNTAX_ERROR)
		return (SYNTAX_ERROR);
	return (SUCCESS);
}
