/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:22:12 by ataan             #+#    #+#             */
/*   Updated: 2025/06/11 07:32:22 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    Things to consider later:
     - free memory before exit
     - signal handling
     - sharing the static functions outside this file

	- Error messages typically go to stderr.
		consider replacing the ft_printf statements
    - When too many args: print error, set exit code to 1,
		AND exit
*/

#include "chicken.h"

static int	all_digits(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	within_bound(const char *s, int neg)
{
	const char	*bound;
	int			i;

	if (neg)
		bound = "9223372036854775808";
	else
		bound = "9223372036854775807";
	i = 0;
	while (*s && s[i])
	{
		if (s[i] < bound[i])
			return (EXIT_FAILURE);
		else if (s[i] > bound[i])
			return (EXIT_SUCCESS);
		i++;
	}
	return (EXIT_FAILURE);
}

//check empty
//check length <= 19
//check digit only
//check within bounds
int	is_valid_long_long(const char *s)
{
	int	neg;

	if (!s || !*s)
		return (0);
	if ((s[0] == '-' || s[0] == '+') && ft_strlen(s) == 1)
		return (0);
	if (s[0] == '-')
		neg = 1;
	else
		neg = 0;
	if (s[0] == '-' || s[0] == '+')
		s++;
	while (*s == '0')
		s++;
	if (*s != '\0' && ft_strlen(s) > 19)
		return (0);
	else
	{
		if (!all_digits(s))
			return (EXIT_SUCCESS);
		if (!within_bound(s, neg))
			return (EXIT_SUCCESS);
	}
	return (1);
}

// exit needs to clean up
int	chkn_exit(char **argv)
{
	if (argv == NULL)
		return (EXIT_FAILURE);
	if (!argv[1])
	{
		printf("exit\n");
		exit(0);
	}
	else if (!is_valid_long_long(argv[1]))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("chicken: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	else if (argv[2])
		ft_putstr_fd("chicken: exit: too many arguments\n", STDERR_FILENO);
	else
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit((unsigned char)ft_atoi(argv[1]));
	}
}
