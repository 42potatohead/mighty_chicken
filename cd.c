/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:09:43 by ataan             #+#    #+#             */
/*   Updated: 2025/06/13 19:17:11 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

/*
    Use chdir(path).
    Handle errors (e.g., invalid path).
    Update $PWD.
    
    tests:
    - cd (with $HOME variable set correctly)
    - cd (with $HOME variable = "") -> do nothing
    - cd .
    - cd ..
    - cd ./
    - cd ../
    - cd existing directory
    - cd non existing directory "bash: cd: ataan: No such file or directory"
    - cd directory with no read permission -> successfully change directory
    - cd filename
    - cd dir1 dir2 "bash: cd: too many arguments"
    - cd /home
*/

int	count_arg(char **argv)
{
	int	argc;

	argc = 0;
	while (argv[argc])
		argc++;
	return (argc);
}

static int	cd_arg_check(char **argv, char ***envp)
{
	int	argc;

	if (envp == NULL || argv == NULL)
	{
		ft_putstr_fd("chicken: empty enviroment\n", 2);
		return (EXIT_FAILURE);
	}
	argc = count_arg(argv);
	if (argc > 2)
	{
		ft_putstr_fd("chicken: cd: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static char	*cd_new_dir(int argc, char **argv, char **envp)
{
	char	*new_dir;

	if (argc == 2)
		new_dir = ft_strdup(argv[1]);
	else
	{
		new_dir = ft_strdup(get_env_var(envp, "HOME"));
		if (new_dir == NULL)
		{
			ft_putstr_fd("chicken: cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	return (new_dir);
}

static char	*cd_old_dir(void)
{
	char	cwd[PATH_MAX];
	char	*old_pwd;

	old_pwd = ft_strdup(getcwd(cwd, sizeof(cwd)));
	if (old_pwd == NULL)
		return (NULL);
	return (old_pwd);
}

/*
    1. count args
    2. save old_dir
    3. find new_dir
    4. change dir
    5. update PWD and OLDPWD
*/
int	chkn_cd(char **argv, char ***envp)
{
	char	cwd[PATH_MAX];
	char	*new_pwd;
	char	*old_pwd;
	int		ret;

	ret = 0;
	if (cd_arg_check(argv, envp))
		return (EXIT_FAILURE);
	old_pwd = cd_old_dir();
	new_pwd = cd_new_dir(count_arg(argv), argv, *envp);
	if (new_pwd == NULL || old_pwd == NULL)
	{
		ft_putstr_fd("chicken: cd: directory error\n", 2);
		return (EXIT_FAILURE);
	}
	ret = chdir(new_pwd);
	if (ret != 0)
		perror("chicken: cd");
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_var(envp, "PWD", cwd);
	set_env_var(envp, "OLDPWD", old_pwd);
	free(old_pwd);
	free(new_pwd);
	return (ret);
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
// 	chkn_cd(argv, &env);
// 	chkn_prnt_envp(argv, env);
// 	// free(env);
// }