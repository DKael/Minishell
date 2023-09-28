/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_parentheses1_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 00:03:08 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 16:07:50 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	heredoc_names_move(t_dll *h_dll, t_dll *tmp_h_dll);
static void	p_data_init(t_data *data, char *cmd);
static void	child_parentheses2(t_data *data, int ao_idx);
extern void	child_heredoc(t_data *data, t_dll *tmp_h_dll);

void	child_parentheses(t_data *data, t_dll *h_dll, char *cmd)
{
	int		ao_idx;
	t_dll	tmp_h_dll;

	heredoc_names_move(h_dll, &tmp_h_dll);
	p_data_init(data, cmd);
	split_cmd(data, data->cmd);
	child_heredoc(data, &tmp_h_dll);
	ao_idx = -1;
	while (++ao_idx < data->ao_cnt)
	{
		if (check_next_ao_cmd(data, ao_idx) == 1)
			continue ;
		dollor_sign_check(data, ao_idx);
		wildcard_check(data, ao_idx);
		if (case_no_pipe_check_isbuiltin(data, ao_idx) == 1)
			return ;
		if (data->pipe_cnt[ao_idx] == 1)
			child(data, ao_idx, 0);
		else
			child_parentheses2(data, ao_idx);
	}
	dll_clear(&tmp_h_dll, str_delete_func);
	c_resource_free_and_exit(data, data->exit_code, T_NULL);
}

static void	heredoc_names_move(t_dll *h_dll, t_dll *tmp_h_dll)
{
	dll_init(tmp_h_dll);
	if (h_dll->size >= 1)
	{
		h_dll->head.back->front = &(tmp_h_dll->head);
		h_dll->tail.front->back = &(tmp_h_dll->tail);
		tmp_h_dll->head.back = h_dll->head.back;
		tmp_h_dll->tail.front = h_dll->tail.front;
		dll_init(h_dll);
	}
}

static void	p_data_init(t_data *data, char *cmd)
{
	int	idx;

	ft_free1((void **)&data->cmd);
	cmd[0] = ' ';
	cmd[ft_strlen(cmd) - 1] = '\0';
	data->cmd = ft_strdup(cmd);
	if (data->cmd == T_NULL)
		c_resource_free_and_exit(data, 1, T_NULL);
	free_2d_array((void ***)&data->ao_split, data->ao_cnt);
	data->ao_cnt = 0;
	idx = -1;
	while (++idx < data->ao_cnt)
		free_2d_dll(&data->tkn[idx], data->pipe_cnt[idx], str_delete_func);
	ft_free1((void **)&data->tkn);
	ft_free1((void **)&data->pipe_cnt);
	ft_free1((void **)&data->logic_table);
	ft_free1((void **)&data->pid_table);
}

static void	child_parentheses2(t_data *data, int ao_idx)
{
	make_pipe(data, ao_idx);
	make_child(data, ao_idx);
	close_pipes(data, data->pipe_cnt[ao_idx]);
	wait_for_child(data, ao_idx);
	free_2d_array2((void ***)&data->pp);
	ft_free1((void **)&data->pid_table);
}
