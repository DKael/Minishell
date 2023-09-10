/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollor_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:58:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/10 20:37:45 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_getenv(t_data *data, const char *name)
{
	char *result;
	t_dllnode *ptr;

	
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

char *get_dollor_parameter(char *instr)
{
	int idx;
	char char_tmp;
	t_bool bad_flag;
	char *result;

	if (instr[1] == '{')
	{
		idx = 1;
		bad_flag = FALSE;
		while (instr[++idx] != '}')
		{
			if (ft_isblank(instr[idx]) == TRUE || instr[idx] == '$')
				bad_flag = TRUE;
			else if (instr[idx] == '\"' || instr[idx] == '\'')
			{
				bad_flag = TRUE;
				char_tmp = instr[idx];
				while (instr[++idx] != char_tmp)
					;
			}
			else if (instr[idx] == '\0')
			{
				printf("minishell: syntax error, unclosed brace\n");
				return (T_NULL);
			}
		}
		if (bad_flag)
		{
			write(2, "minishell: ", 12);
			write(2, instr, idx + 1);
			write(2, ": bad substitution\n", 20);
			return (T_NULL);
		}
		result = (char *)ft_strndup(&instr[2], idx - 2);
	}
	else
	{
		idx = 0;
		while (instr[++idx] != '\0' && instr[idx] != '{'
		&& instr[idx] != '\"' && instr[idx] != '\'' && ft_isblank(instr[idx]) == FALSE)
			;
		result = (char *)ft_strndup(&instr[1], idx - 1);
	}
	if (result == T_NULL)
		return ((char *)-1);
	return (result);
}