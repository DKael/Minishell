/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollor_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:58:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/11 00:17:36 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_getenv(t_data *data, const char *name)
{
	char *temp;
	char *result;
	t_dllnode *ptr;

	if (name[0] == '?' && name[1] == '\0')
	{
		result = ft_itoa(data->wstatus);
		if (result == T_NULL)
			return ((char *)-1);
		return (result);
	}
	temp = getenv(name);
	if (temp != T_NULL)
	{
		result = ft_strdup(temp);
		if (result == T_NULL)
			return ((char *)-1);
		return (result);
	}
	ptr = data->envdll.head.back;
	while (ptr != &(data->envdll.tail))
	{
		if (ft_strcmp(((t_envval *)(ptr->contents))->name, name) == 0)
			return (((t_envval *)(ptr->contents))->value);
		ptr = ptr->back;
	}
	return (T_NULL);
}

char *get_dollor_parameter(char *instr, int *origin_idx)
{
	char char_tmp;
	t_bool bad_flag;
	char *result;
	int idx;

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
		*origin_idx += idx;
	}
	else
	{
		idx = 0;
		while (instr[++idx] != '\0' && instr[idx] != '{' && instr[idx] != '\"'
		&& instr[idx] != '\'' && ft_isblank(instr[idx]) == FALSE && instr[idx] != '$')
			;
		result = (char *)ft_strndup(&instr[1], idx - 1);
		*origin_idx += (idx - 1);
	}
	if (result == T_NULL)
		return ((char *)-1);
	return (result);
}