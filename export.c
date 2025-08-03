/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:11:45 by ataan             #+#    #+#             */
/*   Updated: 2025/06/13 20:14:53 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

/*
	We need to handle these cases:
		- export VAR=value → Set and export a variable.
		- export VAR → Mark an existing variable as exported.
		- export (no arguments) → Print all exported environment variables.
*/

/*
	prints envp for export function (with declare -x)
*/
static int	prnt_envp(char **envp)
{
	int	i;

	if (envp == NULL)
		return (-1);
	i = 0;
	while (envp[i])
		printf("declare -x %s\n", envp[i++]);
	return (EXIT_SUCCESS);
}

// Check if a string is a valid variable name
// Must start with a letter or '_'
// Only letters, numbers, or '_'
static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0] || (!ft_isalpha(name[0]) && name[0] != '_'))
	{
		ft_putstr_fd("chicken_shell: export: `", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
		return (EXIT_SUCCESS);
	}
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			ft_putstr_fd("chicken_shell: export: `", STDERR_FILENO);
			ft_putstr_fd(name, STDERR_FILENO);
			ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	return (EXIT_FAILURE);
}

static int	is_option(char *s)
{
	if (s[0] == '-')
	{
		ft_putstr_fd("chicken: export: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd(": invalid option\n", STDERR_FILENO);
		ft_putstr_fd("export: usage: export name=value\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*
	expects argv input where argv[0] is the command name,
	argv[1...] is command argument
	envp can be a copy of extern char **environ,
	or **envp from the main function or a local copy ...

	TODO: export VAR1=V1 VAR2=V2 VAR3=V3 ... DONE :)
	// Case 1: No arguments → Print all environment variables

	// Case 2: handle export VAR without updating its value
	// TODO:: check if var has a session value,
	might be out of scope
	// Case 3: Set an environment variable (format: VAR=value)
 */
int	chkn_export(char ***envp, char **argv)
{
	int		i;
	char	*equal_sign;
	char	*key;

	if (argv == NULL || envp == NULL)
		return (EXIT_FAILURE);
	if (argv[1] == NULL)
		return (prnt_envp(*envp));
	i = 1;
	while (argv[i])
	{
		if (is_option(argv[i]))
			return (2);
		if (!is_valid_identifier(argv[i]))
			return (EXIT_FAILURE);
		equal_sign = ft_strchr(argv[i], '=');
		if (equal_sign == NULL)
			return (EXIT_SUCCESS);
		key = ft_substr(argv[1], 0, equal_sign - argv[1]);
		if (set_env_var(envp, key, equal_sign + 1) != 0)
			ft_putstr_fd("chicken: export: could not set env variable", 2);
		i++;
		free(key);
	}
	return (EXIT_SUCCESS);
}
