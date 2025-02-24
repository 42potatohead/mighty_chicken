#include <stdio.h>

int main(void)
{
    int i = 0, x = 0;
    while(i < 5)
    {
        printf("i = %d, x = %d\n", i, x);
        // if (x == 5)
        // {
        //     printf("hello\n");
        // }
        // else
        // {
            x++;
            continue;
        // }
        i++;

    }
}
