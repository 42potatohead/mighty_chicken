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

static int count_arg(char **argv)
{
    int argc;

    argc = 0;
    while (argv[argc])
        argc++;
    return argc;
}

static char *find_new_dir(int argc, char **argv, char **envp)
{
    char *new_dir;
     if(argc == 2)
        new_dir = argv[1];
    else
    {
        if (get_env_var(envp, "HOME") == NULL)
            return(EXIT_SUCCESS);
        new_dir = get_env_var(envp, "HOME");
    }
    return new_dir;
}
/*
    1. count args
    2. save old_dir
    3. find new_dir
    4. change dir
    5. update PWD and OLDPWD
*/
int chkn_cd(char **argv, char ***envp)
{
    // ft_printf("chkn_cd called with args: %s %s\n", argv[0], argv[1] ? argv[1] : "NULL");
    ft_printf("chkn_cd called\n");
    char cwd[PATH_MAX];
    char *new_dir;
    int     argc;
    char *old_pwd;

    ft_printf("DEBUG :: BEFORE :: OLDPWD, PWD %s, %s\n", get_env_var(*envp, "OLDPWD"), get_env_var(*envp, "PWD"));
    old_pwd = ft_strdup(getcwd(cwd, sizeof(cwd)));
    ft_printf("old_pwd %s\n", old_pwd);
    argc = count_arg(argv);
    if(argc > 2)
    {
        ft_putstr_fd("chicken: cd: too many arguments\n", 2);
        return (EXIT_FAILURE);
    }
    new_dir = find_new_dir(argc, argv, *envp);
    if (chdir(new_dir) != 0)
    {
        perror("chicken: cd");  // Print error if `chdir` fails
        return(EXIT_FAILURE);
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        set_env_var(envp, "PWD", cwd);
    if (old_pwd)
        set_env_var(envp, "OLDPWD", old_pwd);
    free(old_pwd);  // Clean up heap memory
    ft_printf("DEBUG :: AFTER :: OLDPWD, PWD %s, %s\n", get_env_var(*envp, "OLDPWD"), get_env_var(*envp, "PWD"));
    return (EXIT_SUCCESS);
}
