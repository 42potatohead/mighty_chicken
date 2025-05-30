#include "chicken.h"

// pwd doesnot care for argument, if u do pwd xxx in bash cwd is printed without err
int chkn_pwd() 
{
    char	cwd[PATH_MAX];

    if (getcwd(cwd, PATH_MAX) == NULL)
    {
        perror("getcwd");
        return (1);
    }
    ft_printf("%s\n", cwd);
    return (0);
}