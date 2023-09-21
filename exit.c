
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
            if (!(str[i] >= '0' && str[i] < '9'))
                return (1);
            i++;
        }
    }
    return (0);
}
// argc로 테스트하느라 index값들 한개씩 내려야됨
int ft_exit(char **str)
{
    if (str[1] == 0)
        exit(0);
    else if (only_num(str[1]) == 1)
    {
        err_msg_print3("exit: ", str[1], ": numeric argument required");
        exit(255);
    }
    else if (ft_atoll_check(str[1]) == 1)
    {
        err_msg_print3("exit: ", str[1], ": numeric argument required");
        exit(255);
    }
    if (str[2])
    {
        write(2, "minishell: exit: too many arguments\n", 36);
        return (1); // bash가 꺼지지않음
    }
    if (str[1])
        exit(exit_signal(str[1]));
    exit(0);
}
