#include "chicken.h"
/*
    We need to handle these cases:
        - export VAR=value → Set and export a variable.
        - export VAR → Mark an existing variable as exported.
        - export (no arguments) → Print all exported environment variables.
*/

/*
    prints envp for expor function (with declare -x)
*/
static void prnt_envp(char **envp)
{
    int i;

    i = 0;
    while(envp[i])
        ft_printf("declare -x %s\n", envp[i++]);
}

// Check if a string is a valid variable name
static int is_valid_identifier(const char *name)
{
    int i;

    if (!name || !name[0] || (!ft_isalpha(name[0]) && name[0] != '_'))
        return 0;  // Must start with a letter or '_'
    i = 1;
    while(name[i] && name[i] != '=')
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
        {
            return 0;  // Only letters, numbers, or '_'
        }
        i++;
    }
    return 1;
}

/* 
    expects argv input where argv[0] is the command name, argv[1...] is command argument
    envp can be a copy of extern char **environ, or **envp from the main function or a local copy ... 

    TODO: export VAR1=V1 VAR2=V2 VAR3=V3 ... DONE :)
 */
void chkn_export(char **argv, char ***envp)
{
    int i;
    char *equal_sign;

    if (argv[1] == NULL) // Case 1: No arguments → Print all environment variables
    { 
        prnt_envp(*envp);
        return;
    }
    i = 1;
    while(argv[i])
    {
        printf("DEBUG:: %d\n", is_valid_identifier(argv[i]));
        if (!is_valid_identifier(argv[i]))
        {
            print_stderr("chicken_shell: export: `%s`: not a valid identifier\n", argv[i]);
            return;
        }
        equal_sign = ft_strchr(argv[i], '=');
        if (equal_sign == NULL)  // Case 2: handle export VAR without updating its value TODO:: check if var has a session value, might be out of scope
            return;
        if ( set_env_var(envp, argv[1], equal_sign+1) != 0) // Case 3: Set an environment variable (format: VAR=value)
            perror("chicken: export: ");
        i++;
    }
}
