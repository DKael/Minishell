
#include "minishell.h"

long long ft_atoll_check(const char *str)
{
    long long sign;
    unsigned long long nb;

    nb = 0;
    sign = 1;
    while (*str == '\t' || *str == '\n' || *str == '\v' || *str == '\f' || *str == '\r' || *str == ' ')
        str++;
    if (*str == '-')
        sign *= -1;
    if (*str == '+' || *str == '-')
        str++;
    while (*str >= '0' && *str <= '9')
    {
        if ((nb == 922337203685477580 && (*str - '0' > 7 + ((1 - sign) / 2))) || nb > 922337203685477580)
            return (1);
        nb = nb * 10 + (*str - 48);
        str++;
    }
    return (0);
}

int exit_signal(char *str)
{
    int res;

    res = ft_atoll(str);
    res %= 256;
    if (res < 0)
        res += 256;
    return (res);
}

int only_num(char *str)
{
    int i;

    i = 0;
    if (str != T_NULL)
    {
        while (str[i])
        {
            if (!(str[i] >= '0' && str[i] <= '9'))
                return (1);
            i++;
        }
    }
    return (0);
}

int ft_exit(t_data *data, char **str)
{
    if (str[1] == T_NULL)
        resource_free_and_exit(data, 0, T_NULL);
    else if (only_num(str[1]) == 1)
    {
        err_msg_print3("exit: ", str[1], ": numeric argument required");
        resource_free_and_exit(data, 255, T_NULL);
    }
    else if (ft_atoll_check(str[1]) == 1)
    {
        err_msg_print3("exit: ", str[1], ": numeric argument required");
        resource_free_and_exit(data, 255, T_NULL);
    }
    if (str[2])
    {
        write(2, "minishell: exit: too many arguments\n", 36);
        return (1); 
    }
    if (str[1])
        resource_free_and_exit(data, exit_signal(str[1]), T_NULL);
    resource_free_and_exit(data, 0, T_NULL);
    return (0);
}
