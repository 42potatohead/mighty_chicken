/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:09:22 by ataan             #+#    #+#             */
/*   Updated: 2025/08/01 23:19:06 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

/*
    Prints text to the terminal.
    -n suppresses the newline at the end.

    Check if the first argument is -n.
    Print the remaining arguments accordingly.

    Tests:
    - echo
    - echo hi
    - echo -n
    - echo -n hello
    - echo multiple arguments
    - echo -n multiple arguments
*/

int	chkn_echo(char **argv)
{
	int	i;
	int	err;

	i = 0;
	while (argv[i])
	{
		printf("WOOOOOOOO %s\n", argv[i]);
		i++;
	}
	err = 0;
	if (!argv[1])
		err = printf("\n");
	else
	{
		i = 1;
		while (argv[i] && !ft_strncmp(argv[i], "-n", 2))
			i++;
		while (argv[i])
		{
			err = printf("%s", argv[i]);
			if (argv[i++])
				err = printf(" ");
		}
		if (ft_strncmp(argv[1], "-n", 2))
			err = printf("\n");
	}
	if (err < 0)
		return (1);
	return (0);
}
