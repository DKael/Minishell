/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/12 19:46:45 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cmd_split_error(t_data *data, char *cmd)
{
	int idx1;

	free(cmd);
	dll_clear(&data->envdll, envval_delete_func);
	free_2d_array(&data->ao_split, data->ao_cnt);
	idx1 = -1;
	while (++idx1 < data->ao_cnt)
		free_2d_dll(&data->tkn[idx1][idx2], data->pipe_cnt[idx1], str_delete_func);
	free(data->tkn);
	free(data->pipe_cnt);
	exit(1);
}

void *free_2d_array(char ***arr_ptr, int num)
{
	int idx;
	char **tmp;

	if (*arr_ptr != T_NULL)
	{
		tmp = *arr_ptr;
		idx = -1;
		while (++idx < num)
			free(tmp[idx]);
		free(tmp);
		*arr_ptr = T_NULL;
	}
	return (T_NULL);
}

void	*ft_free1(void *ptr)
{
	free(ptr);
	return (T_NULL);
}

t_bool	ft_free2(void *ptr, t_bool flag)
{
	free(ptr);
	return (flag);
}

void *free_2d_dll(t_dll ***dll_ptr, int num, void (*del)(void *))
{
	int idx;
	t_dll **tmp;
	if (*dll_ptr != T_NULL)
	{
		tmp = *dll_ptr;
		idx = -1;
		while (++idx < num)
		{
			free(tmp[idx]->head.contents);
			dll_clear(tmp[idx], del);
			free(tmp[idx]);
		}
		free(tmp);
		*dll_ptr = T_NULL;
	}
	return (T_NULL);
}

void ignore_quote(char *cmd, int *idx)
{
	char quote;

	quote = cmd[*idx];
	while (cmd[++(*idx)] != quote)
		;
}

void ignore_parentheses(char *cmd, int *idx)
{
	while (cmd[++(*idx)] != ')')
		;
}

void split_cmd(t_data *data, char *cmd)
{
	int idx;

	data->ao_cnt = count_ao_in_cmd(cmd) + 1;
	data->ao_split = split_cmd_by_ao(cmd, data->ao_cnt);
	data->tkn = (t_dll ***)ft_calloc(data->ao_cnt, sizeof(t_dll **));
	data->pipe_cnt = (int *)ft_calloc(data->ao_cnt, sizeof(int));
	if (data->ao_split == T_NULL || data->tkn == T_NULL || data->pipe_cnt == T_NULL)
		cmd_split_error(data, cmd);
	idx = -1;
	while (++idx < data->ao_cnt)
	{
		data->tkn[idx] = tokenize(data->ao_split[idx], &data->pipe_cnt[idx]);
		if (data->tkn[idx] == T_NULL)
			cmd_split_error(data, cmd);
	}
}

int count_ao_in_cmd(char *cmd)
{
	int idx;
	char char_tmp;
	int ao_cnt;

	idx = -1;
	ao_cnt = 0;
	while (cmd[++idx] != '\0')
	{
		if ((cmd[idx] == '|' && cmd[idx + 1] == '|') || (cmd[idx] == '&' && cmd[idx + 1] == '&'))
		{
			ao_cnt++;
			idx++;
		}
		else if (cmd[idx] == '\"' || cmd[idx] == '\'')
			ignore_quote(cmd, &idx);
		else if (cmd[idx] == '(')
			ignore_parentheses(cmd, &idx);
	}
	return (ao_cnt);
}

char **split_cmd_by_ao(char *cmd, int ao_cnt)
{
	char **tmp;
	int ao_idx;
	int idx_chk;
	int idx;

	tmp = (char **)ft_calloc(ao_cnt, sizeof(char *));
	if (tmp == T_NULL)
		return (T_NULL);
	idx_chk = 0;
	ao_idx = 0;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if ((cmd[idx] == '|' && cmd[idx + 1] == '|') || (cmd[idx] == '&' && cmd[idx + 1] == '&'))
		{
			tmp[ao_idx] = ft_strndup(&cmd[idx_chk], idx - idx_chk);
			if (tmp[ao_idx] == T_NULL)
				return (free_2d_array(&tmp, ao_idx));
			idx_chk = idx;
			ao_idx++;
			idx++;
		}
		else if (cmd[idx] == '\"' || cmd[idx] == '\'')
			ignore_quote(cmd, &idx);
		else if (cmd[idx] == '(')
			ignore_parentheses(cmd, &idx);
	}
	tmp[ao_idx] = ft_strndup(&cmd[idx_chk], idx - idx_chk);
	if (tmp[ao_idx] == T_NULL)
		return (free_2d_array(&tmp, ao_idx));
	return (tmp);
}

t_dll **tokenize(char *tkns, int *pipe_cnt)
{
	char **split_tmp;
	t_dll **tkn_part;
	int idx;

	(*pipe_cnt) = count_pipe_in_tkns(tkns) + 1;
	split_tmp = split_tkns_by_pipe(tkns, (*pipe_cnt));
	if (split_tmp == T_NULL)
		return (T_NULL);
	tkn_part = make_dlls((*pipe_cnt));
	if (tkn_part == T_NULL)
		return (free_2d_array(&split_tmp, (*pipe_cnt)));
	idx = -1;
	while (++idx < (*pipe_cnt))
	{
		redirect_count(tkn_part[idx], split_tmp[idx]);
		if (redirect_split(tkn_part[idx], tkns) == FALSE)
		{
			free_2d_dll(&tkn_part, (*pipe_cnt), str_delete_func);
			return (free_2d_array(&split_tmp, (*pipe_cnt)));
		}
		if (parentheses_split(tkn_part[idx], tkns) == FALSE)
		{
			free_2d_dll(&tkn_part, (*pipe_cnt), str_delete_func);
			return (free_2d_array(&split_tmp, (*pipe_cnt)));
		}
		if (remain_split(tkn_part[idx], tkns) == FALSE)
		{
			free_2d_dll(&tkn_part, (*pipe_cnt), str_delete_func);
			return (free_2d_array(&split_tmp, (*pipe_cnt)));
		}
	}
	return (tkn_part);
}

int count_pipe_in_tkns(char *tkns)
{
	int idx;
	char char_tmp;
	int pipe_cnt;

	idx = -1;
	pipe_cnt = 0;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '|')
			pipe_cnt++;
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
	return (pipe_cnt);
}

char **split_tkns_by_pipe(char *tkns, int pipe_cnt)
{
	char **tmp;
	int pipe_idx;
	int idx_chk;
	int idx;

	tmp = (char **)ft_calloc(pipe_cnt, sizeof(char *));
	if (tmp == T_NULL)
		return (T_NULL);
	idx_chk = 0;
	pipe_idx = 0;
	idx = -1;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '|')
		{
			tmp[pipe_idx] = ft_strndup(&tkns[idx_chk], idx - idx_chk);
			if (tmp[pipe_idx] == T_NULL)
				return (free_2d_array(&tmp, pipe_idx));
			idx_chk = idx + 1;
			pipe_idx++;
			idx++;
		}
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
	tmp[pipe_idx] = ft_strndup(&tkns[idx_chk], idx - idx_chk);
	if (tmp[pipe_idx] == T_NULL)
		return (free_2d_array(&tmp, pipe_idx));
	return (tmp);
}

t_dll **make_dlls(int pipe_cnt)
{
	int idx;
	t_dll **tmp;

	tmp = (t_dll **)ft_calloc(pipe_cnt, sizeof(t_dll *));
	if (tmp == T_NULL)
		return (T_NULL);
	idx = -1;
	while (++idx < pipe_cnt)
	{
		tmp[idx] = (t_dll *)ft_calloc(1, sizeof(t_dll));
		if (tmp[idx] == T_NULL)
			return (free_2d_dll(&tmp, idx, T_NULL));
		tmp[idx]->head.contents = ft_calloc(1, sizeof(t_cmd_info));
		if (tmp[idx]->head.contents == T_NULL)
			return (free_2d_array(&tmp, idx + 1));
		dll_init(tmp[idx]);
	}
	return (tmp);
}

t_bool	parentheses_split(t_dll *dll, char *tkns)
{
	int	idx;
	int	idx_chk;
	char	*tmp;
	

	idx = -1;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '(')
		{
			if (((t_cmd_info *)(dll->head.contents))->parentheses_flag == FALSE)
				((t_cmd_info *)(dll->head.contents))->parentheses_flag = TRUE;
			else
				return (syntax_error_print("("));
			while (tkns[++idx] != ')')
				;
			tmp = ft_strndup(&tkns[idx_chk], idx - idx_chk + 1);
			if (tmp == T_NULL)
				return (FALSE);
			if (dll_content_add(dll, (void *)tmp, 1))
				return (ft_free2(tmp, FALSE));
		}
		else if (ft_isblank(tkns[idx]) == FALSE
			&& ((t_cmd_info *)(dll->head.contents))->parentheses_flag == TRUE)
		{
			while (tkns[++idx] != ')' && tkns[++idx] != '(' && tkns[++idx] != '|'
				&& tkns[++idx] != '<' && tkns[++idx] != '>' && tkns[++idx] != '&'
				&& tkns[++idx] != '\0' && ft_isblank(tkns[idx]) == FALSE)
				;
			
		}
	}
}

void redirect_count(t_dll *dll, char *tkns)
{
	int idx;
	t_cmd_info *tmp;

	tmp = (t_cmd_info *)(dll->head.contents);
	idx = -1;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '<' || tkns[idx] == '>')
		{
			tmp->redir_cnt++;
			if (tkns[idx + 1] == '<')
				tmp->heredoc_cnt++;
			if (tkns[idx + 1] == '<' || tkns[idx + 1] == '>')
				idx++;
		}
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
}

// pos[0] = front
// pos[1] = back
// pos[2] = blank
// pos[3] = quote_cnt
void find_front(char *tkns, int *pos, int idx)
{
	pos[0] = idx;
	while (--pos[0] >= 0 && ft_isblank(tkns[pos[0]]) == FALSE && ('0' <= tkns[pos[0]] && tkns[pos[0]] < '9'))
		;
	if (pos[0] >= 0 && ft_isblank(tkns[pos[0]]) == FALSE)
		pos[0] = idx;
	else
		pos[0]++;
}

// pos[0] = front
// pos[1] = back
// pos[2] = blank
// pos[3] = quote_cnt
void find_back_and_calc_blank_quote(char *tkns, int *pos, int idx)
{
	char char_tmp;

	pos[1] = idx;
	pos[2] = 0;
	pos[3] = 0;
	if (tkns[idx + 1] == '<' || tkns[idx + 1] == '>')
		pos[1]++;
	while (ft_isblank(tkns[++pos[1]]) == TRUE)
		pos[2]++;
	while (tkns[pos[1]] != '\0' && ft_isblank(tkns[pos[1]]) == FALSE
		&& tkns[pos[1]] != '<' && tkns[pos[1]] != '>')
	{
		if (tkns[pos[1]] == '\"' || tkns[pos[1]] == '\'')
		{
			pos[3] += 2;
			char_tmp = tkns[pos[1]];
			while (tkns[++pos[1]] != char_tmp)
				;
		}
		pos[1]++;
	}
}

// pos[0] = front
// pos[1] = back
// pos[2] = blank
// pos[3] = quote_cnt
t_bool redirect_split(t_dll *dll, char *tkns)
{
	int idx;
	int pos[4];
	char *tmp;

	idx = -1;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '<' || tkns[idx] == '>')
		{
			find_front(tkns, pos, idx);
			find_back_and_calc_blank_quote(tkns, pos, idx);
			tmp = (char *)ft_calloc(pos[1] - pos[0] - pos[2] - pos[3] + 2, sizeof(char));
			if (tmp == T_NULL)
				return (FALSE);
			redirect_split2_1(tkns, tmp, &pos[0], &pos[1]);
			if (dll_content_add(dll, (void *)tmp, 0) == FALSE)
				return (ft_free2((void *)tmp, FALSE));
			idx = pos[1] - 1;
		}
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
}

void redirect_split2_1(char *tkns, char *tmp, int *front, int *back)
{
	char char_tmp;
	int idx;
	int idx_chk;

	idx_chk = (*front);
	idx = -1;
	while ((*front) < (*back))
	{
		if (tkns[(*front)] == '\"' || tkns[(*front)] == '\'')
		{
			char_tmp = tkns[(*front)];
			while (tkns[++(*front)] != char_tmp)
				tmp[++idx] = tkns[(*front)];
		}
		else if (ft_isblank(tkns[(*front)]) == FALSE)
			redirect_split2_2(tkns, tmp, front, &idx);
		(*front)++;
	}
	while (idx_chk < (*back))
		tkns[idx_chk++] = ' ';
}

void redirect_split2_2(char *tkns, char *tmp, int *front, int *idx)
{
	tmp[++(*idx)] = tkns[(*front)];
	if (tkns[(*front)] == '<' || tkns[(*front)] == '>')
	{
		if (tkns[(*front) + 1] == '<' || tkns[(*front) + 1] == '>')
			tmp[++(*idx)] = tkns[++(*front)];
		tmp[++(*idx)] = ' ';
	}
}

t_bool redirect_split3(t_dll *dll, char *tmp)
{
	t_dllnode *node_ptr;
	
	node_ptr = dll_new_node((void *)tmp);
	if (node_ptr == T_NULL)
	{
		free(tmp);
		return (FALSE);
	}
	dll_add_tail(dll, node_ptr);
	return (TRUE);
}

t_bool remain_split(t_dll *dll, char *tkns)
{

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

cmd_idx = -1;
dll_init(&temp_dll);
while (++cmd_idx < data->cmd_cnt)
{
	data->cmd[cmd_idx] = (char **)ft_calloc(data->cmd_infos[cmd_idx].redir_cnt, sizeof(char *));
	if (data->cmd[cmd_idx] == T_NULL)
	{
		free_2d_array(&tmp, data->cmd_cnt);
		idx = -1;
		while (++idx < cmd_idx)
			free_2d_array(&data->cmd[idx], data->cmd_infos[idx].size);
		free(data->cmd_infos);
		free(data->cmd);
		free(cmd);
		exit(1);
	}
	tmp_ptr = tmp[cmd_idx];
	idx = 0;
	redir_idx = 0;

	printf("tmp_ptr : %s\n", tmp_ptr);
	// after doing all above action, in tmp_ptr string, only command, options, parameters are remain.

	idx = 0;
	while (tmp_ptr[idx] != '\0')
	{
		if (ft_isblank(tmp_ptr[idx]) == FALSE)
		{
			data->cmd_infos[cmd_idx].ip_cnt++;
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
	data->cmd_infos[cmd_idx].size = data->cmd_infos[cmd_idx].redir_cnt + data->cmd_infos[cmd_idx].ip_cnt;
	temp_2d = (char **)ft_calloc(data->cmd_infos[cmd_idx].size, sizeof(char *));
	if (temp_2d == T_NULL)
	{
		free_2d_array(&tmp, data->cmd_cnt);
		idx = -1;
		while (++idx < cmd_idx)
			free_2d_array(&data->cmd[idx], data->cmd_infos[idx].size);
		free_2d_array(&data->cmd[cmd_idx], data->cmd_infos[idx].redir_cnt);
		free(data->cmd);
		free(data->cmd_infos);
		free(cmd);
		exit(1);
	}
	idx = -1;
	while (++idx < data->cmd_infos[cmd_idx].redir_cnt)
		temp_2d[idx] = data->cmd[cmd_idx][idx];
	free(data->cmd[cmd_idx]);
	data->cmd[cmd_idx] = temp_2d;

	ip_idx = data->cmd_infos[cmd_idx].redir_cnt;
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
								free_2d_array(&tmp, data->cmd_cnt);
								free_2d_array(&data->cmd[cmd_idx], ip_idx);
								idx = -1;
								while (++idx < cmd_idx)
									free_2d_array(&data->cmd[idx], data->cmd_infos[idx].size);
								free(data->cmd);
								free(data->cmd_infos);
								free(cmd);
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
						free_2d_array(&tmp, data->cmd_cnt);
						free_2d_array(&data->cmd[cmd_idx], ip_idx);
						idx = -1;
						while (++idx < cmd_idx)
							free_2d_array(&data->cmd[idx], data->cmd_infos[idx].size);
						free(data->cmd);
						free(data->cmd_infos);
						free(cmd);
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
			data->cmd[cmd_idx][ip_idx] = (char *)ft_calloc(idx - idx_chk - quote_cnt + expansion_len + 1, sizeof(char));
			if (data->cmd[cmd_idx][ip_idx] == T_NULL)
			{
				free_2d_array(&tmp, data->cmd_cnt);
				free_2d_array(&data->cmd[cmd_idx], ip_idx);
				idx = -1;
				while (++idx < cmd_idx)
					free_2d_array(&data->cmd[idx], data->cmd_infos[idx].size);
				free(data->cmd);
				free(data->cmd_infos);
				free(cmd);
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
						data->cmd[cmd_idx][ip_idx][++idx2] = str_temp[idx3];
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
								data->cmd[cmd_idx][ip_idx][++idx2] = str_temp[idx3];
							idx_chk += (((t_dollor_tmp *)(node_ptr->contents))->idx_jump - 1);
							node_ptr = node_ptr->back;
						}
						else
							data->cmd[cmd_idx][ip_idx][++idx2] = tmp_ptr[idx_chk];
					}
				}
				else if (tmp_ptr[idx_chk] != '\'')
					data->cmd[cmd_idx][ip_idx][++idx2] = tmp_ptr[idx_chk];
				idx_chk++;
			}
			ip_idx++;
			if (tmp_ptr[idx] == '\0' || ip_idx == data->cmd_infos[cmd_idx].size)
				break;
		}
		idx++;
		dll_clear(&temp_dll, envval_delete_func);
	}
}
free_2d_array(&tmp, data->cmd_cnt);
}
