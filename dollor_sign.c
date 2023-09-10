/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollor_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:58:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/10 18:13:06 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char 	*ft_getenv(t_data *data, const char* name)
{
	char *result;
	t_dllnode *ptr;

	result = getenv(name);
	if (result != T_NULL)
		return (result);
	ptr = data->envdll.head.back;
	while (ptr != &(data->envdll.tail))
	{
		if (ft_strcmp(((t_envval *)(ptr->contents))->name, name) == 0)
			return (((t_envval *)(ptr->contents))->value);
		ptr = ptr->back;
	}
	return (T_NULL);
}

char	*get_dollor_parameter(t_data data, char *instr)
{
	int	idx;
	int	idx_chk;
	t_bool blank_flag;

	if (instr[0] != '$')
		return (T_NULL);
	if (ft_isblank(instr[1]) == TRUE)
		return (T_NULL);
	else if (instr[1] == '{')
	{
		idx = 1;
		blank_flag == FALSE;
		while (instr[++idx] != '}')
		{
			if (blank_flag == FALSE && ft_isblank(instr[idx]) == TRUE)
				blank_flag = TRUE;
			else if (instr[idx] == '\0')
			{
				printf("minishell: syntax error, unclosed brace\n");
				
			}
				
		}
	}
	else
	;
}