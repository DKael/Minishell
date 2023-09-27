/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:57:44 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 14:43:56 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	find_parentheses(char *cmd, int *p_idx);
static	t_bool	pr_in_pr(t_dll *h_names, int *tkn_idx, char *cmd, int *p_idx);
static	t_bool	heredoc_move(t_dll *h_names, t_dll *tmp_dll, int *tkn_idx);

t_bool	parentheses_heredoc(t_dll *h_names, int *tkn_idx, char *cmd)
{
	int			p_idx[2];
	t_dll		tmp_dll;

	cmd[0] = ' ';
	cmd[ft_strlen(cmd) - 1] = ' ';
	if (find_parentheses(cmd, p_idx) == TRUE
		&& pr_in_pr(h_names, tkn_idx, cmd, p_idx) == FALSE)
		return (FALSE);
	dll_init(&tmp_dll);
	if (heredoc_split(&tmp_dll, cmd) == FALSE)
		return (FALSE);
	if (heredoc_move(h_names, &tmp_dll, tkn_idx) == FALSE)
		return (FALSE);
	dll_clear(&tmp_dll, str_delete_func);
	return (TRUE);
}

static t_bool	find_parentheses(char *cmd, int *p_idx)
{
	int	idx;

	p_idx[0] = -1;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '(')
		{
			p_idx[0] = idx;
			ignore_parentheses(cmd, &idx);
			p_idx[1] = idx;
		}
	}
	if (p_idx[0] != -1)
		return (TRUE);
	else
		return (FALSE);
}

static	t_bool	pr_in_pr(t_dll *h_names, int *tkn_idx, char *cmd, int *p_idx)
{
	char	*tmp;

	tmp = ft_strndup(&cmd[p_idx[0]], p_idx[1] - p_idx[0] + 1);
	if (tmp == T_NULL)
		return (FALSE);
	if (parentheses_heredoc(h_names, tkn_idx, tmp) == FALSE)
		return (ft_free2((void **)&tmp, FALSE));
	ft_free1((void **)&tmp);
	return (TRUE);
}

static	t_bool	heredoc_move(t_dll *h_names, t_dll *tmp_dll, int *tkn_idx)
{
	t_dllnode	*ptr[2];
	char		*tmp;

	ptr[1] = h_names->head.back;
	ptr[0] = tmp_dll->head.back;
	while (ptr[0] != &(tmp_dll->tail))
	{
		tmp = ft_strstr((char *)(ptr[0]->contents), "<<");
		if (tmp == T_NULL)
		{
			ptr[0] = ptr[0]->back;
			continue ;
		}
		if (heredoc_make1_2(h_names, ptr[1], tkn_idx, tmp + 3) == FALSE)
		{
			dll_clear(tmp_dll, str_delete_func);
			return (FALSE);
		}
		ptr[0] = ptr[0]->back;
	}
	return (TRUE);
}

void	heredoc_unlink(t_data *data)
{
	int			idx1;
	int			idx2;
	t_dll		*ptr;
	t_dllnode	*node_ptr;

	if (data->ao_cnt == 0 || data->pipe_cnt == T_NULL)
		return ;
	idx1 = -1;
	while (++idx1 < data->ao_cnt)
	{
		idx2 = -1;
		while (++idx2 < data->pipe_cnt[idx1])
		{
			ptr = &(((t_cmd_info *)((data->tkn[idx1][idx2])->head.contents))->heredoc_names);
			node_ptr = ptr->head.back;
			while (node_ptr != &(ptr->tail))
			{
				unlink((char *)node_ptr->contents);
				node_ptr = node_ptr->back;
			}
		}
	}
}
