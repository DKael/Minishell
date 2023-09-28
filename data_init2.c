/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 09:32:33 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 14:34:06 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envval	*make_envval(t_data *data, char *envp, int idx)
{
	t_envval	*env;

	env = T_NULL;
	if (envp[idx] == '=')
	{
		env = (t_envval *)ft_calloc(1, sizeof(t_envval));
		if (env == T_NULL)
		{
			dll_clear(&data->envdll, envval_delete_func);
			message_exit("minishell: malloc error\n", 1);
		}
		env->name = ft_strndup((char *)envp, idx);
		if (ft_strncmp(envp, "SHLVL", idx) == 0)
		{
			if (ft_isdecimal(&envp[idx + 1]) == TRUE)
				env->value = ft_itoa(ft_atoi(&envp[idx + 1]) + 1);
			else
				env->value = ft_itoa(1);
		}
		else if (ft_strncmp(envp, "OLDPWD", idx) == 0)
			;
		else
			env->value = ft_strdup(&envp[idx + 1]);
	}
	return (env);
}

void	check_pwd_oldpwd_exist(t_data *data)
{
	char	*tmp[3];

	tmp[0] = "export";
	tmp[1] = "OLDPWD";
	tmp[2] = T_NULL;
	if (ft_export(&data->envdll, &data->sorted_envdll, tmp) == -1)
	{
		dll_clear(&data->envdll, envval_delete_func);
		dll_clear(&data->sorted_envdll, T_NULL);
		message_exit("minishell: malloc error\n", 1);
	}
	tmp[1] = ft_strjoin("PWD=", data->wd);
	tmp[2] = T_NULL;
	if (ft_export(&data->envdll, &data->sorted_envdll, tmp) == -1)
	{
		free(tmp[1]);
		dll_clear(&data->envdll, envval_delete_func);
		dll_clear(&data->sorted_envdll, T_NULL);
		message_exit("minishell: malloc error\n", 1);
	}
	free(tmp[1]);
}
