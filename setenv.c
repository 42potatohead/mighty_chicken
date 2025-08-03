/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:04:18 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:33:27 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

static char	*join_key_value(const char *key, const char *value)
{
	char	*tmp;
	char	*new_var;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	new_var = ft_strjoin(tmp, value);
	if (!new_var)
		return (NULL);
	free(tmp);
	return (new_var);
}

static int	replace_existing_var(char ***envp, const char *key, char *new_var)
{
	int		i;
	size_t	key_len;
	char	**env;

	key_len = ft_strlen(key);
	env = *envp;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (0);
		}
		i++;
	}
	return (i);
}

static int	append_new_var(char *new_var, int i, char ***envp)
{
	char	**new_env;
	char	**env;
	int		j;

	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(new_var);
		return (-1);
	}
	env = *envp;
	j = 0;
	while (j < i)
	{
		new_env[j] = env[j];
		j++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	*envp = new_env;
	return (0);
}

/*
	free(new_var); -> not needed  ...
	we shall clean the whole enviroment when shell is closed
*/
int	set_env_var(char ***envp, const char *key, const char *value)
{
	int		i;
	char	*new_var;

	if (envp == NULL || key == NULL || value == NULL)
		return (EXIT_FAILURE);
	new_var = join_key_value(key, value);
	if (!new_var)
		return (EXIT_FAILURE);
	i = replace_existing_var(envp, key, new_var);
	if (i > 0)
		return (append_new_var(new_var, i, envp));
	return (EXIT_SUCCESS);
}

char	*get_env_var(char **envp, const char *key)
{
	size_t	key_len;
	int		i;

	if (!envp || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
		{
			return (envp[i] + key_len + 1);
		}
		i++;
	}
	return (NULL);
}
