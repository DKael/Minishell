/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:09:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 00:10:48 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin_func(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (2);
	else if (ft_strcmp(cmd, "export") == 0)
		return (3);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (4);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (5);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (6);
	else if (ft_strcmp(cmd, "env") == 0)
		return (7);
	else
		return (0);
}

int	execute_builtin_func(int func_idx, char **argu_lst, t_data *data)
{
	int	result;
	
	if (func_idx == 1)
		result = ft_echo(argu_lst);
	else if (func_idx == 2)
		result = ft_cd(data, argu_lst);
	else if (func_idx == 3)
		result = ft_export(&data->envdll, &data->sorted_envdll, argu_lst);
	else if (func_idx == 4)
		result = ft_unset(&data->envdll, &data->sorted_envdll, argu_lst);
	else if (func_idx == 5)
		result = ft_pwd(data);
	else if (func_idx == 6)
		result = ft_exit(data, argu_lst);
	else
		result = ft_env(data, argu_lst);
	if (result == 0 || result == 1)
		return (result);
	else
		builtin_func_error(data, result);
	return (1);
}

static void builtin_func_error(t_data *data, int result)
{
	if (result == 4)
		resource_free_and_exit(data, 1, "stat() error");
	else if (result == -1)
		resource_free_and_exit(data, 1, "malloc error");
	else if (result == -2)
		resource_free_and_exit(data, 1, "chdir error");
}
