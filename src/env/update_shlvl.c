#include "../../includes/minishell.h"

bool	is_all_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-' && i == 0)
			i++;
		else if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

char	*free_shlvl_vars(char **lvl_str, char **tmp)
{
	if (*lvl_str != NULL)
	{
		free(*lvl_str);
		*lvl_str = NULL;
	}
	if (*tmp != NULL)
	{
		free(*tmp);
		*tmp = NULL;
	}
	return (NULL);
}

char	*get_shlvl_var(char **shlvl_split)
{
	int		lvl;
	char	*lvl_str;
	char	*tmp;	
	char	*shlvl_var;

	lvl = ft_atoi(shlvl_split[1]) + 1;
	if (lvl < 0)
		return (ft_strdup("SHLVL=0"));
	lvl_str = ft_itoa(lvl);
	if (lvl_str == NULL)
		return (NULL);
	tmp = ft_strjoin(shlvl_split[0], "=");
	if (tmp == NULL)
		return (free_shlvl_vars(&lvl_str, &tmp));
	shlvl_var = ft_strjoin(tmp, lvl_str);
	if (shlvl_var == NULL)
		return (free_shlvl_vars(&lvl_str, &tmp));
	free(lvl_str);
	free(tmp);
	return (shlvl_var);
}

char	*update_shlvl(char *shlvl, int *flag)
{
	char	**shlvl_split;
	char	*shlvl_var;

	*flag = ON;
	shlvl_split = ft_split(shlvl, '=');
	if (shlvl_split == NULL)
		return (NULL);
	if (is_all_digit(shlvl_split[1]) && shlvl_split[2] == NULL)
		shlvl_var = get_shlvl_var(shlvl_split);
	else
		shlvl_var = ft_strdup("SHLVL=1");
	if (shlvl_var == NULL)
	{
		free_2d_array(&shlvl_split);
		return (NULL);
	}
	free_2d_array(&shlvl_split);
	return (shlvl_var);
}
