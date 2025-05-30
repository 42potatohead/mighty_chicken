#include "chicken.h"

/* prints enviroment variables for the built-in env */
void chkn_prnt_envp(char **envp)
{
    int i;

    i = 0;
    while(envp[i])
    {
        ft_printf("%s\n", envp[i]);
        i++;
    }
}