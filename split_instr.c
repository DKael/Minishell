/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_instr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/11 00:12:38 by hyungdki         ###   ########.fr       */
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
	t_dll temp_dll;
	int ip_idx;
	int expansion_len;
	t_dollor_tmp *dollor_tmp;
	t_dllnode *node_ptr;
	char **temp_2d;
	int dollor_idx;
	char *str_temp;

	instr_idx = -1;
	dll_init(&temp_dll);
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
				while (--front >= 0 && ('0' <= tmp_ptr[front] && tmp_ptr[front] < '9') && ft_isblank(tmp_ptr[front]) == FALSE)
					;
				if (front >= 0 && ft_isblank(tmp_ptr[front]) == FALSE)
					front = idx;
				else
					front++;
				back = idx;
				if (tmp_ptr[idx + 1] == '<' || tmp_ptr[idx + 1] == '>')
					back++;
				blank = 0;
				quote_cnt = 0;
				while (ft_isblank(tmp_ptr[++back]) == TRUE)
					blank++;
				while (tmp_ptr[back] != '\0')
				{
					if (ft_isblank(tmp_ptr[back]) == TRUE || tmp_ptr[back] == '<' || tmp_ptr[back] == '>')
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
					else if (ft_isblank(tmp_ptr[front]) == FALSE)
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

		idx = 0;
		while (tmp_ptr[idx] != '\0')
		{
			if (ft_isblank(tmp_ptr[idx]) == FALSE)
			{
				data->instr_infos[instr_idx].ip_cnt++;
				while (ft_isblank(tmp_ptr[idx]) == FALSE && tmp_ptr[idx] != '\0')
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

		ip_idx = data->instr_infos[instr_idx].redir_cnt;
		idx = 0;
		while (tmp_ptr[idx] != '\0')
		{
			if (ft_isblank(tmp_ptr[idx]) == FALSE)
			{
				quote_cnt = 0;
				expansion_len = 0;
				idx_chk = idx;
				while (ft_isblank(tmp_ptr[idx]) == FALSE && tmp_ptr[idx] != '\0')
				{
					if (tmp_ptr[idx] == '\"' || tmp_ptr[idx] == '\'')
					{
						quote_cnt += 2;
						char_tmp = tmp_ptr[idx];
						while (tmp_ptr[++idx] != char_tmp)
						{
							if (char_tmp == '\"' && tmp_ptr[idx] == '$' && ft_isblank(tmp_ptr[idx + 1]) == FALSE)
							{
								dollor_idx = idx;
								dollor_tmp = (t_dollor_tmp *)malloc(sizeof(t_dollor_tmp));
								if (dollor_tmp == T_NULL)
									exit(1);
								dollor_tmp->name = get_dollor_parameter(&tmp_ptr[idx], &idx);
								if (dollor_tmp->name == (char *)-1)
									exit(1);
								else if (dollor_tmp->name == T_NULL)
								{
									free_2d_array(&tmp, data->instr_cnt);
									free_2d_array(&data->instr[instr_idx], ip_idx);
									idx = -1;
									while (++idx < instr_idx)
										free_2d_array(&data->instr[idx], data->instr_infos[idx].size);
									free(data->instr);
									free(data->instr_infos);
									free(instr);
									return;
								}
								dollor_tmp->value = ft_getenv(data, dollor_tmp->name);
								dollor_tmp->idx_jump = (idx - dollor_idx + 1);
								expansion_len -= (idx - dollor_idx + 1);
								if (dollor_tmp->value != T_NULL)
									expansion_len += ft_strlen(dollor_tmp->value);
								else if (dollor_tmp->value == (char *)-1)
									exit(1);
								else
									dollor_tmp->value = "";
								node_ptr = dll_new_node((void *)dollor_tmp);
								if (node_ptr == T_NULL)
									exit(1);
								dll_add_tail(&temp_dll, node_ptr);
							}
						}
					}
					else if (tmp_ptr[idx] == '$' && ft_isblank(tmp_ptr[idx + 1]) == FALSE && tmp_ptr[idx + 1] != '\0')
					{
						dollor_idx = idx;
						dollor_tmp = (t_dollor_tmp *)malloc(sizeof(t_dollor_tmp));
						if (dollor_tmp == T_NULL)
							exit(1);
						dollor_tmp->name = get_dollor_parameter(&tmp_ptr[idx], &idx);
						if (dollor_tmp->name == (char *)-1)
							exit(1);
						else if (dollor_tmp->name == T_NULL)
						{
							free_2d_array(&tmp, data->instr_cnt);
							free_2d_array(&data->instr[instr_idx], ip_idx);
							idx = -1;
							while (++idx < instr_idx)
								free_2d_array(&data->instr[idx], data->instr_infos[idx].size);
							free(data->instr);
							free(data->instr_infos);
							free(instr);
							return;
						}
						dollor_tmp->value = ft_getenv(data, dollor_tmp->name);
						dollor_tmp->idx_jump = (idx - dollor_idx + 1);
						expansion_len -= (idx - dollor_idx + 1);
						if (dollor_tmp->value != T_NULL)
							expansion_len += ft_strlen(dollor_tmp->value);
						else if (dollor_tmp->value == (char *)-1)
							exit(1);
						else
							dollor_tmp->value = "";
						node_ptr = dll_new_node((void *)dollor_tmp);
						if (node_ptr == T_NULL)
							exit(1);
						dll_add_tail(&temp_dll, node_ptr);
					}
					idx++;
				}
				data->instr[instr_idx][ip_idx] = (char *)ft_calloc(idx - idx_chk - quote_cnt + expansion_len + 1, sizeof(char));
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
				int idx3;

				idx2 = -1;
				node_ptr = temp_dll.head.back;
				while (idx_chk < idx)
				{
					if (tmp_ptr[idx_chk] == '$')
					{
						str_temp = ((t_dollor_tmp *)(node_ptr->contents))->value;
						idx3 = -1;
						while (str_temp[++idx3] != '\0')
							data->instr[instr_idx][ip_idx][++idx2] = str_temp[idx3];
						idx_chk += (((t_dollor_tmp *)(node_ptr->contents))->idx_jump - 1);
						node_ptr = node_ptr->back;
					}
					else if (tmp_ptr[idx_chk] == '\"')
					{
						while (tmp_ptr[++idx] != '\"')
						{
							if (tmp_ptr[idx_chk] == '$')
							{
								str_temp = ((t_dollor_tmp *)(node_ptr->contents))->value;
								idx3 = -1;
								while (str_temp[++idx3] != '\0')
									data->instr[instr_idx][ip_idx][++idx2] = str_temp[idx3];
								idx_chk += (((t_dollor_tmp *)(node_ptr->contents))->idx_jump - 1);
								node_ptr = node_ptr->back;
							}
							else
								data->instr[instr_idx][ip_idx][++idx2] = tmp_ptr[idx_chk];
						}
					}
					else if (tmp_ptr[idx_chk] != '\'')
						data->instr[instr_idx][ip_idx][++idx2] = tmp_ptr[idx_chk];
					idx_chk++;
				}
				ip_idx++;
				if (tmp_ptr[idx] == '\0' || ip_idx == data->instr_infos[instr_idx].size)
					break;
			}
			idx++;
			dll_clear(&temp_dll, envval_delete_func);
		}
	}
	free_2d_array(&tmp, data->instr_cnt);
}
