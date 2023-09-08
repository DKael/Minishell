/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_instr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/08 14:09:07 by hyungdki         ###   ########.fr       */
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
	char char_tmp;

	idx = 0;
	while (instr[idx] != '\0')
	{
		if (instr[idx] == '|')
		{
			if (instr[idx + 1] != '|')
				data->instr_cnt++;
			else if (instr[idx + 1] == '|')
				idx++;
		}
		else if (instr[idx] == '\"' || instr[idx] == '\'')
		{
			char_tmp = instr[idx];
			while (instr[++idx] != char_tmp)
				;
		}
		idx++;
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
	idx = 0;
	while (instr[idx] != '\0')
	{
		if (instr[idx] == '|')
		{
			if (instr[idx + 1] == '|')
			{
				idx += 2;
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
		else if (instr[idx] == '\"' || instr[idx] == '\'')
		{
			char_tmp = instr[idx];
			while (instr[++idx] != char_tmp)
				;
		}
		idx++;
	}
	tmp[instr_idx] = ft_strndup(&instr[idx_chk], idx - idx_chk);
	if (tmp[instr_idx] == T_NULL)
	{
		free_2d_array(&tmp, instr_idx);
		free(instr);
		exit(1);
	}

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
		idx = 0;
		while (tmp_ptr[idx] != '\0')
		{
			if (tmp_ptr[idx] == '<' || tmp_ptr[idx] == '>')
			{
				data->instr_infos[instr_idx].redir_cnt++;
				if (tmp_ptr[idx + 1] == '<')
					data->instr_infos[instr_idx].heredoc_cnt++;
				if (tmp_ptr[idx + 1] == '<' || tmp_ptr[idx + 1] == '>')
					idx++;
			}
			else if (tmp_ptr[idx] == '\"' || tmp_ptr[idx] == '\'')
			{
				char_tmp = tmp_ptr[idx];
				while (tmp_ptr[++idx] != char_tmp)
					;
			}
			idx++;
		}
	}

	int redir_idx;
	int idx2;
	int quote_cnt;

	instr_idx = -1;
	while (++instr_idx < data->instr_cnt)
	{
		data->instr[instr_idx] = (char **)ft_calloc(data->instr_infos[instr_idx].redir_cnt, sizeof(char *));
		if (data->instr[instr_idx] == T_NULL)
		{
			free_2d_array(&tmp, data->instr_cnt);
			idx = -1;
			while (++idx < instr_idx)
				free_2d_array(&data->instr[idx], data->instr_infos[idx].size);
			free(data->instr_infos);
			free(data->instr);
			free(instr);
			exit(1);
		}
		tmp_ptr = tmp[instr_idx];
		idx = 0;
		redir_idx = 0;
		while (tmp_ptr[idx] != '\0')
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
				quote_cnt = 0;
				while ((9 <= tmp_ptr[++back] && tmp_ptr[back] <= 13) || tmp_ptr[back] == ' ')
					blank++;
				while (tmp_ptr[back] != '\0')
				{
					if (((9 <= tmp_ptr[back] && tmp_ptr[back] <= 13) || tmp_ptr[back] == ' ') || tmp_ptr[back] == '<' || tmp_ptr[back] == '>')
						break;
					else if (tmp_ptr[back] == '\"' || tmp_ptr[back] == '\'')
					{
						quote_cnt += 2;
						char_tmp = tmp_ptr[back];
						while (tmp_ptr[++back] != char_tmp)
							;
					}
					back++;
				}
				data->instr[instr_idx][redir_idx] = (char *)ft_calloc(back - front - blank - quote_cnt + 2, sizeof(char));
				if (data->instr[instr_idx][redir_idx] == T_NULL)
				{
					free_2d_array(&tmp, data->instr_cnt);
					free_2d_array(&data->instr[instr_idx], redir_idx);
					idx = -1;
					while (++idx < instr_idx)
						free_2d_array(&data->instr[idx], data->instr_infos[idx].size);
					free(data->instr);
					free(data->instr_infos);
					free(instr);
					exit(1);
				}
				idx2 = -1;
				while (front < back)
				{
					if (tmp_ptr[front] == '\"' || tmp_ptr[front] == '\'')
					{
						char_tmp = tmp_ptr[front];
						tmp_ptr[front] = ' ';
						while (tmp_ptr[++front] != char_tmp)
						{
							data->instr[instr_idx][redir_idx][++idx2] = tmp_ptr[front];
							tmp_ptr[front] = ' ';
						}
						tmp_ptr[front] = ' ';
					}
					else if (!((9 <= tmp_ptr[front] && tmp_ptr[front] <= 13) || tmp_ptr[front] == ' '))
					{
						data->instr[instr_idx][redir_idx][++idx2] = tmp_ptr[front];
						if (tmp_ptr[front] == '<' || tmp_ptr[front] == '>')
						{
							tmp_ptr[front] = ' ';
							if (tmp_ptr[front + 1] == '<' || tmp_ptr[front + 1] == '>')
							{
								data->instr[instr_idx][redir_idx][++idx2] = tmp_ptr[++front];
								tmp_ptr[front] = ' ';
							}
							data->instr[instr_idx][redir_idx][++idx2] = ' ';
						}
						else
							tmp_ptr[front] = ' ';
					}
					front++;
				}
				redir_idx++;
				idx = back - 1;
			}
			else if (tmp_ptr[idx] == '\"' || tmp_ptr[idx] == '\'')
			{
				char_tmp = tmp_ptr[idx];
				while (tmp_ptr[++idx] != char_tmp)
					;
			}
			idx++;
		}
		printf("tmp_ptr : %s\n", tmp_ptr);
		// after doing all above action, in tmp_ptr string, only command, options, parameters are remain.

		char **temp_2d;

		idx = 0;
		while (tmp_ptr[idx] != '\0')
		{
			if (!((9 <= tmp_ptr[idx] && tmp_ptr[idx] <= 13) || tmp_ptr[idx] == ' '))
			{
				data->instr_infos[instr_idx].ip_cnt++;
				while (!((9 <= tmp_ptr[idx] && tmp_ptr[idx] <= 13) || tmp_ptr[idx] == ' ') && tmp_ptr[idx] != '\0')
				{
					if (tmp_ptr[idx] == '\"' || tmp_ptr[idx] == '\'')
					{
						char_tmp = tmp_ptr[idx];
						while (tmp_ptr[++idx] != char_tmp)
							;
					}
					idx++;
				}
				if (tmp_ptr[idx] == '\0')
					break;
			}
			idx++;
		}
		data->instr_infos[instr_idx].size = data->instr_infos[instr_idx].redir_cnt + data->instr_infos[instr_idx].ip_cnt;
		temp_2d = (char **)ft_calloc(data->instr_infos[instr_idx].size, sizeof(char *));
		if (temp_2d == T_NULL)
		{
			free_2d_array(&tmp, data->instr_cnt);
			idx = -1;
			while (++idx < instr_idx)
				free_2d_array(&data->instr[idx], data->instr_infos[idx].size);
			free_2d_array(&data->instr[instr_idx], data->instr_infos[idx].redir_cnt);
			free(data->instr);
			free(data->instr_infos);
			free(instr);
			exit(1);
		}
		idx = -1;
		while (++idx < data->instr_infos[instr_idx].redir_cnt)
			temp_2d[idx] = data->instr[instr_idx][idx];
		free(data->instr[instr_idx]);
		data->instr[instr_idx] = temp_2d;

		int ip_idx;
		int idx_chk;

		ip_idx = data->instr_infos[instr_idx].redir_cnt;
		idx = 0;
		while (tmp_ptr[idx] != '\0')
		{
			if (!((9 <= tmp_ptr[idx] && tmp_ptr[idx] <= 13) || tmp_ptr[idx] == ' '))
			{
				quote_cnt = 0;
				idx_chk = idx;
				while (!((9 <= tmp_ptr[idx] && tmp_ptr[idx] <= 13) || tmp_ptr[idx] == ' ') && tmp_ptr[idx] != '\0')
				{
					if (tmp_ptr[idx] == '\"' || tmp_ptr[idx] == '\'')
					{
						quote_cnt += 2;
						char_tmp = tmp_ptr[idx];
						while (tmp_ptr[++idx] != char_tmp)
							;
					}
					idx++;
				}
				data->instr[instr_idx][ip_idx] = (char *)ft_calloc(idx - idx_chk - quote_cnt + 1,sizeof(char));
				if (data->instr[instr_idx][ip_idx] == T_NULL)
				{
					free_2d_array(&tmp, data->instr_cnt);
					free_2d_array(&data->instr[instr_idx], ip_idx);
					idx = -1;
					while (++idx < instr_idx)
						free_2d_array(&data->instr[idx], data->instr_infos[idx].size);
					free(data->instr);
					free(data->instr_infos);
					free(instr);
					exit(1);
				}
				idx2 = -1;
				while (idx_chk < idx)
				{
					if (tmp_ptr[idx_chk] != '\"' && tmp_ptr[idx_chk] != '\'')
						data->instr[instr_idx][ip_idx][++idx2] = tmp_ptr[idx_chk];
					idx_chk++;
				}
				ip_idx++;
				if (tmp_ptr[idx] == '\0' || ip_idx == data->instr_infos[instr_idx].size)
					break;
			}
			idx++;
		}
	}
	free_2d_array(&tmp, data->instr_cnt);
}
