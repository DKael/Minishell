#include "minishell.h"

void    print_str(char **str, int i)
{
    if (i == 1)
    {
        while(str[i])
        {
            printf("%s", str[i++]);
            if (str[i])
                printf(" ");
        }
        printf("\n");
    }
    else
    {
        while(str[i])
        {
            printf("%s", str[i++]);
            if (str[i])
                printf(" ");
        }
    }
}

int    ft_echo(char **str)
{
    if (!(ft_strcmp(str[1], "-n")))
        print_str(str, 2);
    else
        print_str(str, 1);
    return (0);
}
