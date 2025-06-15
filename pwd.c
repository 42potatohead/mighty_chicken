/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:26:46 by ataan             #+#    #+#             */
/*   Updated: 2025/06/11 14:26:47 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

static int	is_option(char *s)
{
	if (s[0] == '-')
	{
		ft_putstr_fd("chicken: pwd: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd(": invalid option\n", STDERR_FILENO);
		ft_putstr_fd("pwd: usage: pwd", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/* pwd doesnot care for argument,
	if u do pwd xxx in bash cwd is printed without err */
int	chkn_pwd(char **argv)
{
	char	cwd[PATH_MAX];
	int		i;

	if (argv == NULL)
		return (1);
	i = 0;
	while (argv[i])
	{
		if (is_option(argv[i]))
			return (2);
		i++;
	}
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	ft_printf("%s\n", cwd);
	return (0);
}
