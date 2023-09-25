/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollor_sign2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:15:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 14:43:26 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_dollor_parameter2(char *cmd, int *idx);

char	*ft_getenv(t_data *data, const char *name)
{
	t_dllnode	*ptr;

	if (name[0] == '?' && name[1] == '\0')
		return (data->exit_code_str);
	ptr = data->envdll.head.back;
	while (ptr != &(data->envdll.tail))
	{
		if (ft_strcmp(((t_envval *)(ptr->contents))->name, name) == 0)
			return (((t_envval *)(ptr->contents))->value);
		ptr = ptr->back;
	}
	return (T_NULL);
}

char	*get_dollor_parameter(char *cmd, int *idx)
{
	char	*result;
	int		idx_chk;

	if (cmd[(*idx) + 1] == '{')
	{
		idx_chk = (*idx) + 2;
		(*idx) += 1;
		while (cmd[++(*idx)] != '}')
			;
		return ((char *)ft_strndup(&cmd[idx_chk], (*idx) - idx_chk));
	}
	else if (cmd[(*idx) + 1] == '?')
	{
		(*idx)++;
		return ((char *)ft_strdup("?"));
	}
	else
		return (get_dollor_parameter2(cmd, idx));
}

static char	*get_dollor_parameter2(char *cmd, int *idx)
{
	char	*result;
	int		idx_chk;

	idx_chk = (*idx) + 1;
	while (cmd[++(*idx)] != '\0' && cmd[(*idx)] != '{'
		&& cmd[(*idx)] != '\"' && cmd[(*idx)] != '\''
		&& ft_isblank(cmd[(*idx)]) == FALSE && cmd[(*idx)] != '$')
		;
	result = (char *)ft_strndup(&cmd[idx_chk], (*idx) - idx_chk);
	(*idx)--;
	return (result);
}
