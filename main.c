#include "includes/minishell.h"

void	free_env_and_exit(t_env *env)
{
	if (env->vars != NULL)
		ft_lstclear(&env->vars, &free);
	if (env->exp != NULL)
		ft_lstclear(&env->exp, &free);
	if (env->paths != NULL)
		free_2d_array(&env->paths);
	if (env->saved_pwd != NULL)
		free(env->saved_pwd);
	exit(1);
}

void	prompt(void)
{
	ft_putstr_fd("minishell$ ", STDOUT_FILENO);
}

int	run_shell(t_env *env)
{
	char	*line;

	line = NULL;
	while (true)
	{
		if (catch_signals(env) == FATAL_ERROR)
			return (FATAL_ERROR);
		prompt();
		if (get_next_line_eof(STDIN_FILENO, &line) == FATAL_ERROR)
			return (free_and_ret(&line, FATAL_ERROR));
		if (!empty_line(line))
		{
			if (prep_input(env, line) == FATAL_ERROR)
				return (free_and_ret(&line, FATAL_ERROR));
		}
		free_str(&line);
	}
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	env;

	ft_bzero(&env, sizeof(t_env));
	if (save_env(&env, envp) == FATAL_ERROR)
		free_env_and_exit(&env);
	if (run_shell(&env) == FATAL_ERROR)
		free_env_and_exit(&env);
	(void)argv;
	(void)argc;
	return (SUCCESS);
}
