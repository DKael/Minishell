/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollor_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:58:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/12 22:25:36 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_getenv(t_data *data, const char *name)
{
	t_dllnode *ptr;

	if (name[0] == '?' && name[1] == '\0')
		return (data->last_exit_code_str);
	ptr = data->envdll.head.back;
	while (ptr != &(data->envdll.tail))
	{
		if (ft_strcmp(((t_envval *)(ptr->contents))->name, name) == 0)
			return (((t_envval *)(ptr->contents))->value);
		ptr = ptr->back;
	}
	return (T_NULL);
}

char *get_dollor_parameter(char *cmd, int *origin_idx)
{
	char *result;
	int idx;

	if (cmd[1] == '{')
	{
		idx = 1;
		while (cmd[++idx] != '}')
			;
		result = (char *)ft_strndup(&cmd[2], idx - 2);
		*origin_idx += idx;
	}
	else
	{
		idx = 0;
		while (cmd[++idx] != '\0' && cmd[idx] != '{' && cmd[idx] != '\"'
		&& cmd[idx] != '\'' && ft_isblank(cmd[idx]) == FALSE && cmd[idx] != '$')
			;
		result = (char *)ft_strndup(&cmd[1], idx - 1);
		*origin_idx += (idx - 1);
	}
	return (result);
}