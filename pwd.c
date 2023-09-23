#include "minishell.h"

int ft_pwd(t_data *data)
{
   char path[4096];
   char *flag;

    flag = getcwd(path, 4096);
    if (flag == T_NULL)
    {
        if (data->wd[0] == '\0')
            write(2, "pwd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 109);
        else
            printf("%s\n", data->wd);
    }
    else
        printf ("%s\n", path);
    return (0);
}


