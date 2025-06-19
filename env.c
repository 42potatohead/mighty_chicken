/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:09:05 by ataan             #+#    #+#             */
/*   Updated: 2025/06/11 07:29:33 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

/* prints enviroment variables for the built-in env */
int	chkn_prnt_envp(char **argv, char **envp)
{
	int	i;

	if (envp == NULL || argv == NULL)
		return (EXIT_FAILURE);
	i = 0;
	if (argv[i] != NULL)
		i++;
	if (i > 1) // make an invalid option err
	{
		ft_putstr_fd("chicken: env: env does not take arguments\n", STDERR_FILENO);
		return (1);
	}
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
