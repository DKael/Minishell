/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 00:09:12 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *free_2d_dll(t_dll ***dll_ptr, int num, void (*del)(void *));
int count_ao_in_cmd(char *cmd);
char **split_cmd_by_ao(char *cmd, int ao_cnt, t_logic *logic_table);
t_dll **tokenize(char *tkns, int *pipe_cnt);
int count_pipe_in_tkns(char *tkns);
char **split_tkns_by_pipe(char *tkns, int pipe_cnt);
t_dll **make_dlls(int pipe_cnt);
void redirect_count(t_dll *dll, char *tkns);
void find_front(char *tkns, int *pos, int idx);
void find_back_and_calc_blank_quote(char *tkns, int *pos, int idx);
t_bool redirect_split(t_dll *dll, char *tkns);
void redirect_split2_1(char *tkns, char *tmp, int *pos, t_bool heredoc_flag);
void redirect_split2_2(char *tkns, char *tmp, int *pos, t_bool heredoc_flag);
t_bool parentheses_split(t_dll *dll, char *tkns);
void remain_count(t_dll *dll, char *tkns);
t_bool remain_split(t_dll *dll, char *tkns);
t_bool remain_split2(t_dll *dll, char *tkns, int *idx);

void cmd_split_error(t_data *data, char *cmd, char *msg)
{
	int idx1;

	printf("minishell: %s\n", msg);
	ft_free1((void **)&cmd);
	dll_clear(&data->envdll, envval_delete_func);
	free_2d_array((void ***)&data->ao_split, data->ao_cnt);
	idx1 = -1;
	while (++idx1 < data->ao_cnt)
		free_2d_dll(&data->tkn[idx1], data->pipe_cnt[idx1], str_delete_func);
	ft_free1((void **)&data->tkn);
	ft_free1((void **)&data->pipe_cnt);
	ft_free1((void **)&data->logic_table);
	
	exit(1);
}

void *free_2d_array(void ***arr_ptr, int num)
{
	int idx;
	void **tmp;

	if (*arr_ptr != T_NULL)
	{
		tmp = *arr_ptr;
		idx = -1;
		while (++idx < num)
			ft_free1((void **)&tmp[idx]);
		ft_free1((void **)&tmp);
		*arr_ptr = T_NULL;
	}
	return (T_NULL);
}

void *free_2d_array2(void ***arr_ptr)
{
	int idx;
	void **tmp;

	if (*arr_ptr != T_NULL)
	{
		tmp = *arr_ptr;
		idx = -1;
		while (tmp[++idx] != T_NULL)
			ft_free1((void **)&tmp[idx]);
		ft_free1((void **)&tmp);
		*arr_ptr = T_NULL;
	}
	return (T_NULL);
}

void *ft_free1(void **ptr)
{
	free(*ptr);
	*ptr = T_NULL;
	return (T_NULL);
}

t_bool ft_free2(void **ptr, t_bool flag)
{
	free(*ptr);
	*ptr = T_NULL;
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
			if (tmp[idx] != T_NULL)
			{
				dll_clear(&((t_cmd_info *)(tmp[idx]->head.contents))->heredoc_names, del);
				ft_free1((void **)&tmp[idx]->head.contents);
				dll_clear(tmp[idx], del);
				ft_free1((void **)&tmp[idx]);
			}
		}
		ft_free1((void **)&tmp);
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
	int	layer;

	layer = 0;
	while (1)
	{
		(*idx)++;
		if (cmd[(*idx)] == ')')
		{
			if (layer == 0)
				break;
			else
				layer--;
		}
		else if (cmd[(*idx)] == '(')
			layer++;
		else if (cmd[(*idx)] == '\'' || cmd[(*idx)] == '\"')
			ignore_quote(cmd, idx);
	}
}

void split_cmd(t_data *data, char *cmd)
{
	int idx;

	data->ao_cnt = count_ao_in_cmd(cmd) + 1;
	data->logic_table = (t_logic *)ft_calloc(data->ao_cnt - 1, sizeof(t_logic));
	if (data->logic_table == T_NULL)
	{
		ft_free1((void **)&cmd);
		exit(1);
	}
	data->ao_split = split_cmd_by_ao(cmd, data->ao_cnt, data->logic_table);

	// for (int i = 0; i < data->ao_cnt - 1; i++)
	// {
	// 	if (data->logic_table[i] == OR)
	// 		printf("OR ");
	// 	else
	// 		printf("AND ");
	// }
	// printf("\n\n");
	// for (int i = 0; i < data->ao_cnt; i++)
	// 	printf("data->ao_split[%d] : %s\n", i, data->ao_split[i]);

	data->tkn = (t_dll ***)ft_calloc(data->ao_cnt, sizeof(t_dll **));
	data->pipe_cnt = (int *)ft_calloc(data->ao_cnt, sizeof(int));
	if (data->ao_split == T_NULL || data->tkn == T_NULL || data->pipe_cnt == T_NULL)
		cmd_split_error(data, cmd, "malloc error");
	idx = -1;
	while (++idx < data->ao_cnt)
	{
		data->tkn[idx] = tokenize(data->ao_split[idx], &data->pipe_cnt[idx]);
		if (data->tkn[idx] == T_NULL)
			cmd_split_error(data, cmd, "malloc error");
	}
}

int count_ao_in_cmd(char *cmd)
{
	int idx;
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

char **split_cmd_by_ao(char *cmd, int ao_cnt, t_logic *logic_table)
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
			if (cmd[idx] == '|' && cmd[idx + 1] == '|')
				logic_table[ao_idx] = OR;
			else if (cmd[idx] == '&' && cmd[idx + 1] == '&')
				logic_table[ao_idx] = AND;
			tmp[ao_idx] = ft_strndup(&cmd[idx_chk], idx - idx_chk);
			if (tmp[ao_idx] == T_NULL)
				return (free_2d_array((void ***)&tmp, ao_idx));
			idx_chk = idx + 2;
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
		return (free_2d_array((void ***)&tmp, ao_idx));
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
		return (free_2d_array((void ***)&split_tmp, (*pipe_cnt)));

	// printf("\n\ntkns : %s\n", tkns);
	// for (int i = 0; i < (*pipe_cnt); i++)
	// 	printf("_%s_\n", split_tmp[i]);
	// printf("\n");

	idx = -1;
	while (++idx < (*pipe_cnt))
	{
		// redirect_count(tkn_part[idx], split_tmp[idx]);
		if (redirect_split(tkn_part[idx], split_tmp[idx]) == FALSE)
			break;
		// printf("after redirection split\n");
		// printf("split_tmp[%d] : %s\n", idx, split_tmp[idx]);
		// dll_print(tkn_part[idx], dll_str_print_func);

		if (parentheses_split(tkn_part[idx], split_tmp[idx]) == FALSE)
			break;
		// printf("after parentheses split\n");
		// printf("split_tmp[%d] : %s\n", idx, split_tmp[idx]);
		// dll_print(tkn_part[idx], dll_str_print_func);

		// remain_count(tkn_part[idx], split_tmp[idx]);
		if (remain_split(tkn_part[idx], split_tmp[idx]) == FALSE)
			break;
		// printf("after remain split\n");
		// printf("split_tmp[%d] : %s\n", idx, split_tmp[idx]);
		// dll_print(tkn_part[idx], dll_str_print_func);
	}
	free_2d_array((void ***)&split_tmp, (*pipe_cnt));
	if (idx < (*pipe_cnt))
		return (free_2d_dll(&tkn_part, (*pipe_cnt), str_delete_func));
	return (tkn_part);
}

int count_pipe_in_tkns(char *tkns)
{
	int idx;
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
				return (free_2d_array((void ***)&tmp, pipe_idx));
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
		return (free_2d_array((void ***)&tmp, pipe_idx));
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
		dll_init(tmp[idx]);
		tmp[idx]->head.contents = ft_calloc(1, sizeof(t_cmd_info));
		if (tmp[idx]->head.contents == T_NULL)
		{
			ft_free1((void **)&tmp[idx]);
			return (free_2d_dll(&tmp, idx, T_NULL));
		}
		dll_init(&((t_cmd_info *)(tmp[idx]->head.contents))->heredoc_names);
	}
	return (tmp);
}

// void redirect_count(t_dll *dll, char *tkns)
// {
// 	int idx;
// 	t_cmd_info *tmp;

// 	tmp = (t_cmd_info *)(dll->head.contents);
// 	idx = -1;
// 	while (tkns[++idx] != '\0')
// 	{
// 		if (tkns[idx] == '<' || tkns[idx] == '>')
// 		{
// 			tmp->redir_cnt++;
// 			if (tkns[idx + 1] == '<')
// 				tmp->heredoc_cnt++;
// 			if (tkns[idx + 1] == '<' || tkns[idx + 1] == '>')
// 				idx++;
// 		}
// 		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
// 			ignore_quote(tkns, &idx);
// 		else if (tkns[idx] == '(')
// 			ignore_parentheses(tkns, &idx);
// 	}
// }

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
	while (tkns[pos[1]] != '\0' && ft_isblank(tkns[pos[1]]) == FALSE && tkns[pos[1]] != '<' && tkns[pos[1]] != '>')
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
	char *con;
	t_cmd_info *tmp;
	t_bool	heredoc_flag;

	tmp = (t_cmd_info *)(dll->head.contents);
	idx = -1;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '<' || tkns[idx] == '>')
		{
			if (tkns[idx + 1] == '<')
				heredoc_flag = TRUE;
			else
				heredoc_flag = FALSE;
			tmp->redir_cnt++;
			find_front(tkns, pos, idx);
			find_back_and_calc_blank_quote(tkns, pos, idx);
			con = (char *)ft_calloc(pos[1] - pos[0] - pos[2] - pos[3] + 2, sizeof(char));
			if (con == T_NULL)
				return (FALSE);
			redirect_split2_1(tkns, con, pos, heredoc_flag);
			if (dll_content_add(dll, (void *)con, 0) == FALSE)
				return (ft_free2((void **)&con, FALSE));
			idx = pos[1] - 1;
		}
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
	return (TRUE);
}

void redirect_split2_1(char *tkns, char *tmp, int *pos, t_bool heredoc_flag)
{
	char char_tmp;
	int idx_chk;

	idx_chk = pos[0];
	pos[2] = -1;
	while (pos[0] < pos[1])
	{
		if (tkns[pos[0]] == '\"' || tkns[pos[0]] == '\'')
		{
			char_tmp = tkns[pos[0]];
			while (tkns[++pos[0]] != char_tmp)
			{
				if (tkns[pos[0]] == '$' && char_tmp == '\"' && !heredoc_flag)
					tkns[pos[0]] = -1;
				tmp[++pos[2]] = tkns[pos[0]];
			}
		}
		else if (ft_isblank(tkns[pos[0]]) == FALSE)
			redirect_split2_2(tkns, tmp, pos, heredoc_flag);
		pos[0]++;
	}
	while (idx_chk < pos[1])
		tkns[idx_chk++] = ' ';
}

void redirect_split2_2(char *tkns, char *tmp, int *pos, t_bool heredoc_flag)
{
	if (tkns[pos[0]] == '$' && !heredoc_flag)
		tkns[pos[0]] = -1;
	tmp[++pos[2]] = tkns[pos[0]];
	if (tkns[pos[0]] == '<' || tkns[pos[0]] == '>')
	{
		if (tkns[pos[0] + 1] == '<' || tkns[pos[0] + 1] == '>')
			tmp[++pos[2]] = tkns[++pos[0]];
		tmp[++pos[2]] = ' ';
	}
}

t_bool parentheses_split(t_dll *dll, char *tkns)
{
	int idx;
	int idx_chk;
	char *tmp;
	int layer;

	idx = -1;
	layer = 0;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '(')
		{
			((t_cmd_info *)(dll->head.contents))->parentheses_flag = TRUE;
			idx_chk = idx;
			while (1)
			{
				idx++;
				if (tkns[idx] == ')')
				{
					if (layer == 0)
						break;
					else
						layer--;
				}
				else if (tkns[idx] == '(')
					layer++;
				else if (tkns[idx] == '\'' || tkns[idx] == '\"')
					ignore_quote(tkns, &idx);
			}
			tmp = ft_strndup(&tkns[idx_chk], idx - idx_chk + 1);
			if (tmp == T_NULL)
				return (FALSE);
			if (dll_content_add(dll, (void *)tmp, 0) == FALSE)
				return (ft_free2((void **)&tmp, FALSE));
			while (idx_chk <= idx)
				tkns[idx_chk++] = ' ';
		}
	}
	return (TRUE);
}

// void remain_count(t_dll *dll, char *tkns)
// {
// 	int idx;
// 	t_cmd_info *tmp;

// 	tmp = (t_cmd_info *)(dll->head.contents);
// 	idx = -1;
// 	while (tkns[++idx] != '\0')
// 	{
// 		if (ft_isblank(tkns[idx]) == FALSE)
// 		{
// 			tmp->cp_cnt++;
// 			while (ft_isblank(tkns[idx]) == FALSE && tkns[idx] != '\0')
// 			{
// 				if (tkns[idx] == '\"' || tkns[idx] == '\'')
// 					ignore_quote(tkns, &idx);
// 				idx++;
// 			}
// 			if (tkns[idx] == '\0')
// 				break;
// 		}
// 	}
// 	tmp->size = tmp->redir_cnt + tmp->cp_cnt;
// }

t_bool remain_split(t_dll *dll, char *tkns)
{
	int idx[3];
	t_cmd_info *tmp;

	tmp = (t_cmd_info *)(dll->head.contents);
	idx[0] = 0;
	while (tkns[idx[0]] != '\0')
	{
		if (ft_isblank(tkns[idx[0]]) == FALSE)
		{
			tmp->cp_cnt++;
			idx[2] = 0;
			idx[1] = idx[0];
			while (ft_isblank(tkns[idx[0]]) == FALSE && tkns[idx[0]] != '\0')
			{
				if (tkns[idx[0]] == '\"' || tkns[idx[0]] == '\'')
				{
					idx[2] += 2;
					ignore_quote(tkns, &idx[0]);
				}
				idx[0]++;
			}
			if (remain_split2(dll, tkns, idx) == FALSE)
				return (FALSE);
		}
		else
			idx[0]++;
	}
	tmp->size = tmp->redir_cnt + tmp->cp_cnt;
	return (TRUE);
}

t_bool remain_split2(t_dll *dll, char *tkns, int *idx)
{
	char *tmp;
	int idx2;
	char char_tmp;

	tmp = (char *)ft_calloc(idx[0] - idx[1] - idx[2] + 1, sizeof(char));
	if (tmp == T_NULL)
		return (FALSE);
	idx2 = -1;
	while (idx[1] < idx[0])
	{
		if (tkns[idx[1]] == '\'' || tkns[idx[1]] == '\"')
		{
			char_tmp = tkns[idx[1]];
			while (tkns[++idx[1]] != char_tmp)
			{
				if (tkns[idx[1]] == '$' && char_tmp == '\"')
					tkns[idx[1]] = -1;
				tmp[++idx2] = tkns[idx[1]];
			}
		}
		else
		{
			if (tkns[idx[1]] == '$')
				tkns[idx[1]] = -1;
			tmp[++idx2] = tkns[idx[1]];
		}

		idx[1]++;
	}
	if (dll_content_add(dll, (void *)tmp, 0) == FALSE)
		return (ft_free2((void **)&tmp, FALSE));
	return (TRUE);
}
