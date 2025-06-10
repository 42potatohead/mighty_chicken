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

int chkn_echo(char **argv)
{
    int i;

    if(!argv[1]) //just echo
        ft_printf("\n");
    else
    {
        i = 1;
        if (!ft_strncmp(argv[1], "-n", 2))
            i = 2;
        while (argv[i])
        {
            ft_printf("%s", argv[i]);
            if(argv[i++])
                ft_printf(" ");
        }
        if(ft_strncmp(argv[1], "-n", 2))
            ft_printf("\n");
    }
    return (0);
}
