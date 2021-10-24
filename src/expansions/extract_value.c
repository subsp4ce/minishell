#include "../../includes/minishell.h"

int	get_len_substr(char *str, int i, t_vars *var)
{
	int		len;

	len = 0;
	while (str[i])
	{
		quote_state(&var->state, str[i]);
		if ((str[i] == '$' && is_expansion(&var->state, str, i))
			|| (str[i] == '~' && is_home_expansion(str, i)))
			return (len);
		len++;
		i++;
	}
	return (len);
}

char	*get_var_value(t_env *env, char *var_name)
{
	t_list	*lst;
	char	*value;
	int		len;

	lst = env->vars;
	value = NULL;
	len = 0;
	while (lst != NULL)
	{
		if (ft_strncmp(lst->content, var_name, ft_strlen(var_name)) == 0)
		{
			len = ft_strlen(lst->content);
			value = ft_substr(lst->content, ft_strlen(var_name),
					len - ft_strlen(var_name));
			if (value == NULL)
				return (NULL);
		}
		lst = lst->next;
	}
	return (value);
}

void	home_not_set_error(void)
{
	ft_putstr_fd("minishell: HOME not set. Unable ", STDERR_FILENO);
	ft_putstr_fd("to perform `~' expansion. Restore HOME ", STDERR_FILENO);
	ft_putstr_fd("variable to regain this functionality\n", STDERR_FILENO);
	g_exit_status = 1;
}

bool	has_expansion_value(t_env *env, char *var_name)
{
	t_list	*lst;

	lst = env->vars;
	while (lst != NULL)
	{
		if (strncmp(lst->content, var_name, ft_strlen(var_name)) == 0)
			return (true);
		lst = lst->next;
	}
	return (false);
}

char	*extract_value(t_env *env, char *name, char c)
{
	char	*value;

	value = NULL;
	if (has_expansion_value(env, name))
		value = get_var_value(env, name);
	else if (c == '~')
	{
		home_not_set_error();
		value = ft_strdup("~");
	}
	else
		value = ft_strdup("");
	return (value);
}
