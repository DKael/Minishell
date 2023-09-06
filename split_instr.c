/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_instr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/06 22:47:01 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_2d_array(char ***arr_ptr, int num)
{
	int idx;
	char **arr;

	if (*arr_ptr != T_NULL)
	{
		arr = *arr_ptr;
		idx = -1;
		while (++idx < num)
			free(arr[idx]);
		free(arr);
		*arr_ptr = T_NULL;
	}
}

char ***split_instr(t_data *data, char *instr)
{
	int idx;
	int idx_chk;

	idx = -1;
	while (instr[++idx] != '\0')
	{
		if (instr[idx] == '|')
		{
			if (instr[idx + 1] != '|')
				data->pipe_cnt++;
			idx++;
		}
		else if (instr[idx] == '\"')
			while (instr[++idx] != '\"')
				;
		else if (instr[idx] == '\'')
			while (instr[++idx] != '\'')
				;
	}

	char **temp;
	int instr_idx;

	temp = (char **)ft_calloc(data->pipe_cnt + 1, sizeof(char *));
	if (temp == T_NULL)
		return (T_NULL);
	idx_chk = 0;
	instr_idx = 0;
	idx = -1;
	while (instr[++idx] != '\0')
	{
		if (instr[idx] == '|')
		{
			if (instr[idx + 1] == '|')
			{
				idx++;
				continue;
			}
			temp[instr_idx] = ft_strndup(&instr[idx_chk], idx - idx_chk);
			if (temp[instr_idx] == T_NULL)
			{
				free_2d_array(&temp, instr_idx);
				free(instr);
				exit(1);
			}
			idx_chk = idx + 1;
			instr_idx++;
		}
		else if (instr[idx] == '\"')
			while (instr[++idx] != '\"')
				;
		else if (instr[idx] == '\'')
			while (instr[++idx] != '\'')
				;
	}
	temp[instr_idx] = ft_strndup(&instr[idx_chk], idx - idx_chk);
	if (temp[instr_idx] == T_NULL)
	{
		free_2d_array(&temp, instr_idx);
		free(instr);
		exit(1);
	}

	// for (int i = 0; i < data->pipe_cnt + 1; i++)
	// 	printf("%s\n", temp[i]);

	// free_2d_array(&temp, data->pipe_cnt + 1);
	
	char ***result;

	result = (char ***)malloc(sizeof(char **) * data->pipe_cnt);
	if (result == T_NULL)
	{
		free(temp);
		return (T_NULL);
	}


	return (T_NULL);
}
