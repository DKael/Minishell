/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:06:50 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 02:41:27 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(t_data *data)
{
   char path[MAX_PATH_LEN];
   char *flag;

    flag = getcwd(path, MAX_PATH_LEN);
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
