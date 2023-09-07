/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_instr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/07 23:38:32 by hyungdki         ###   ########.fr       */
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

void split_instr(t_data *data, char *instr)
{
	int idx;
	int idx_chk;

	idx = -1;
	while (instr[++idx] != '\0')
	{
		if (instr[idx] == '|')
		{
			if (instr[idx + 1] != '|')
				data->instr_cnt++;
			idx++;
		}
		else if (instr[idx] == '\"')
			while (instr[++idx] != '\"')
				;
		else if (instr[idx] == '\'')
			while (instr[++idx] != '\'')
				;
	}
	data->instr_cnt++;

	char **tmp;
	int instr_idx;

	tmp = (char **)ft_calloc(data->instr_cnt, sizeof(char *));
	if (tmp == T_NULL)
	{
		free(instr);
		exit(1);
	}
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
			tmp[instr_idx] = ft_strndup(&instr[idx_chk], idx - idx_chk);
			if (tmp[instr_idx] == T_NULL)
			{
				free_2d_array(&tmp, instr_idx);
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
	tmp[instr_idx] = ft_strndup(&instr[idx_chk], idx - idx_chk);
	if (tmp[instr_idx] == T_NULL)
	{
		free_2d_array(&tmp, instr_idx);
		free(instr);
		exit(1);
	}

	// for (int i = 0; i < data->pipe_cnt + 1; i++)
	// 	printf("%s\n", tmp[i]);

	// free_2d_array(&tmp, data->pipe_cnt + 1);

	data->instr = (char ***)ft_calloc(data->instr_cnt, sizeof(char **));
	if (data->instr == T_NULL)
	{
		free_2d_array(&tmp, data->instr_cnt);
		free(instr);
		exit(1);
	}
	data->instr_infos = (t_instr_info *)ft_calloc(data->instr_cnt, sizeof(t_instr_info));
	if (data->instr_infos == T_NULL)
	{
		free_2d_array(&tmp, data->instr_cnt);
		free(data->instr);
		free(instr);
		exit(1);
	}

	// redirection check
	char *tmp_ptr;
	int front;
	int back;
	int blank;

	instr_idx = -1;
	while (++instr_idx < data->instr_cnt)
	{
		tmp_ptr = tmp[instr_idx];
		printf("instruction : %s\n", tmp_ptr);
		idx = -1;
		while (tmp_ptr[++idx] != '\0')
		{
			if (tmp_ptr[idx] == '<' || tmp_ptr[idx] == '>')
			{
				data->instr_infos[instr_idx].redir_cnt++;
				if (tmp_ptr[idx + 1] == '<')
					data->instr_infos[instr_idx].heredoc_cnt++;
				if (tmp_ptr[idx + 1] == '<' || tmp_ptr[idx + 1] == '>')
					idx++;
			}
			else if (tmp_ptr[idx] == '\"')
				while (tmp_ptr[++idx] != '\"')
					;
			else if (tmp_ptr[idx] == '\'')
				while (tmp_ptr[++idx] != '\'')
					;
		}
	}

	int redir_idx;
	int idx2;

	instr_idx = -1;
	while (++instr_idx < data->instr_cnt)
	{
		data->instr[instr_idx] = (char **)ft_calloc(data->instr_infos[instr_idx].redir_cnt + 1, sizeof(char *));
		if (data->instr[instr_idx] == T_NULL)
		{
			free_2d_array(&tmp, data->instr_cnt);
			idx = -1;
			while (++idx < instr_idx)
				free_2d_array(&data->instr[idx], data->instr_infos[idx].redir_cnt + 1);
			free(data->instr);
			free(instr);
			exit(1);
		}
		tmp_ptr = tmp[instr_idx];
		idx = -1;
		redir_idx = 0;
		while (tmp_ptr[++idx] != '\0')
		{
			if (tmp_ptr[idx] == '<' || tmp_ptr[idx] == '>')
			{
				front = idx;
				while (--front >= 0 && ('0' <= tmp_ptr[front] && tmp_ptr[front] < '9') && tmp_ptr[front] != ' ')
					;
				if (front != -1 && tmp_ptr[front] != ' ')
					front = idx;
				else
					front++;
				back = idx;
				if (tmp_ptr[idx + 1] == '<' || tmp_ptr[idx + 1] == '>')
					back++;
				blank = 0;
				while ((9 <= tmp_ptr[++back] && tmp_ptr[back] <= 13) || tmp_ptr[back] == ' ')
					blank++;
				while (!((9 <= tmp_ptr[++back] && tmp_ptr[back] <= 13) || tmp_ptr[back] == ' ') && tmp_ptr[back] != '<' && tmp_ptr[back] != '>' && tmp_ptr[back] != '\0')
					;
				data->instr[instr_idx][redir_idx] = (char *)ft_calloc(back - front - blank + 1, sizeof(char));
				if (data->instr[instr_idx][redir_idx] == T_NULL)
				{
					free_2d_array(&tmp, data->instr_cnt);
					idx = -1;
					while (++idx < instr_idx + 1)
						free_2d_array(&data->instr[idx], data->instr_infos[idx].redir_cnt + 1);
					free(data->instr);
					free(instr);
					exit(1);
				}
				idx2 = -1;
				while (front < back)
				{
					if (!((9 <= tmp_ptr[front] && tmp_ptr[front] <= 13) || tmp_ptr[front] == ' '))
					{
						data->instr[instr_idx][redir_idx][++idx2] = tmp_ptr[front];
						tmp_ptr[front] = ' ';
					}
					front++;
				}
				redir_idx++;
				idx = back - 1;
			}
			else if (tmp_ptr[idx] == '\"')
				while (tmp_ptr[++idx] != '\"')
					;
			else if (tmp_ptr[idx] == '\'')
				while (tmp_ptr[++idx] != '\'')
					;
		}
		// after doing all above action, in tmp_ptr string, only command, options, parameters are remain.
		data->instr[instr_idx][redir_idx] = tmp_ptr;
	}
	free(tmp);
}
