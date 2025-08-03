/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:05:18 by ataan             #+#    #+#             */
/*   Updated: 2025/08/01 23:05:19 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

int	is_relative(char *cmd, t_grand *grand)
{
	if (ft_strncmp(cmd, "/", 1) == 0 || ft_strncmp(cmd, "./", 2) == 0
		|| ft_strncmp(cmd, "../", 3) == 0)
	{
		grand->env.full_path = ft_strdup(cmd);
		return (1);
	}
	return (0);
}

void	get_path(t_grand *grand, char *cmd)
{
	int		i;
	char	*cmd_with_slash;
	char	*temp_path;

	if (is_relative(cmd, grand))
		return ;
	grand->env.split_path = ft_split(get_env_var(grand->env.envp, "PATH"), ':');
	cmd_with_slash = ft_strjoin("/", cmd);
	i = 0;
	while (grand->env.split_path && grand->env.split_path[i])
	{
		temp_path = ft_strjoin(grand->env.split_path[i], cmd_with_slash);
		if (access(temp_path, X_OK) == 0)
		{
			grand->env.full_path = temp_path;
			break ;
		}
		free(temp_path);
		i++;
	}
	if (!grand->env.full_path)
		grand->env.full_path = ft_strdup(cmd);
	free(cmd_with_slash);
}
