/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:04:14 by ataan             #+#    #+#             */
/*   Updated: 2025/06/13 20:20:46 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

/*
    If no options are supplied,
    each name refers to a variable; if there is no variable by that name,
	a function with that name, if any, is unset.

    Readonly variables and functions may not be unset.
    Some shell variables lose their special behavior if they are unset;
    such behavior is noted in the description of the individual variables.
    The return status is zero unless a name is readonly or may not be unset.

    tests:
    - unset
    - unset non existing variable
    - unset multiple variables
    - unset VAR
*/

static void	unset_var(char ***envp, const char *key)
{
	size_t	len;
	char	**env;
	int		i;
	int		j;

	env = *envp;
	len = ft_strlen(key);
	i = 0;
	j = 0;
	while (len != 0 && env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
		{
			i++;
			continue;
		}
		env[j++] = env[i++];
	}
	env[j] = NULL;
}
// add is option check
int	chkn_unset(char ***envp, char **argv)
{
	int	i;

	if (envp == NULL || argv == NULL)
		return (EXIT_FAILURE);
	if (argv[1] == NULL)
		return (EXIT_SUCCESS);
	i = 1;
	while (argv[i])
	{
		unset_var(envp, argv[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
// char **copy_env(char **envp) {
//     int count = 0;
//     while (envp[count]) count++;

//     char **new_env = malloc(sizeof(char *) * (count + 1));
//     if (!new_env) return (NULL);

//     for (int i = 0; i < count; i++)
//         new_env[i] = strdup(envp[i]);
//     new_env[count] = NULL;
//     return (new_env);
// }

// int main(int argc, char **argv, char **envp)
// {
// 	char **env;

// 	env = copy_env(envp);
// 	char *arg[] = {"aaa=99", "aaa=99", NULL};
// 	chkn_export(&envp, arg);
// 	chkn_prnt_envp(arg, envp);
// 	arg[1] = "aaa";
// 	chkn_unset(&envp, arg);
// 	chkn_prnt_envp(arg, envp);
// 	return (EXIT_SUCCESS);
// }
