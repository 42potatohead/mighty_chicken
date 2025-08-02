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

void	get_path(t_grand *grand, char *cmd)
{
	int	i;

	if (ft_strncmp(cmd, "/", 1) == 0 || ft_strncmp(cmd, "./", 2) == 0
		|| ft_strncmp(cmd, "../", 3) == 0)
		grand->env.full_path = cmd;
	else
	{
		grand->env.split_path = ft_split(get_env_var(grand->env.envp, "PATH"),
				':');
		i = 0;
		cmd = ft_strjoin("/", cmd);
		while (grand->env.split_path && grand->env.split_path[i])
		{
			grand->env.full_path = ft_strjoin(grand->env.split_path[i], cmd);
			if (access(grand->env.full_path, X_OK) == 0)
				break ;
			else
				grand->env.full_path = cmd;
			i++;
		}
	}
}
