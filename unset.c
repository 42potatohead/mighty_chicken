#include "chicken.h"

/*
    If no options are supplied, 
    each name refers to a variable; if there is no variable by that name, a function with that name, if any, is unset.
    
    Readonly variables and functions may not be unset. Some shell variables lose their special behavior if they are unset;
    such behavior is noted in the description of the individual variables. The return status is zero unless a name is readonly
    or may not be unset.
     
    tests:
    - unset
    - unset non existing variable
    - unset multiple variables
    - unset VAR
*/

static void unset_var(char ***envp, const char *key) 
{
    size_t len;
    char **env;
    int i;
    int j;
        
    env = *envp;
    len = ft_strlen(key);
    i = 0;
    j = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=') // Check if entry starts with "NAME=" exactly
        {
            i++; // skip this one (unset it)
            continue;
        }
        env[j++] = env[i++];
    }
    env[j] = NULL;
}

int chkn_unset(char ***envp, char **argv)
{
    int i;
ft_printf("before %s\n", argv[1]);
    if (argv[1] == NULL) // Case 1: No arguments → Print all environment variables
        return (EXIT_SUCCESS);
    i = 1;
    while(argv[i])
    {
        unset_var(envp, argv[i]);
        i++;
    }
ft_printf("after %s\n", get_env_var(*envp, argv[1]));
    return(EXIT_SUCCESS);
}
