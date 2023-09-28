/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_parentheses2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:43:43 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 13:42:06 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_heredoc2(t_data *data, t_dll *tmp_h_dll, int *idx);
static void	child_heredoc3(t_data *data, t_dll *tmp_h_dll, int *idx);

void	child_heredoc(t_data *data, t_dll *tmp_h_dll)
{
	int			idx[3];

	idx[0] = -1;
	child_heredoc2(data, tmp_h_dll, idx);
	idx[0] = -1;
	child_heredoc3(data, tmp_h_dll, idx);
}

static void	child_heredoc2(t_data *data, t_dll *tmp_h_dll, int *idx)
{
	t_cmd_info	*info;
	t_dllnode	*ptr;

	while (++idx[0] < data->ao_cnt)
	{
		idx[1] = -1;
		while (++idx[1] < data->pipe_cnt[idx[0]])
		{
			info = (t_cmd_info *)(data->tkn[idx[0]][idx[1]]->head.contents);
			ptr = data->tkn[idx[0]][idx[1]]->head.back;
			idx[2] = -1;
			while (++idx[2] < info->redir_cnt)
			{
				if (ft_strstr((char *)(ptr->contents), "<<") == T_NULL)
				{
					ptr = ptr->back;
					continue ;
				}
				dll_node_move_to_another_dll_tail(tmp_h_dll->head.back,
					tmp_h_dll, &info->heredoc_names);
				ptr = ptr->back;
			}
		}
	}
}

static void	child_heredoc3(t_data *data, t_dll *tmp_h_dll, int *idx)
{
	t_cmd_info	*info;
	char		*tmp_str;

	while (++idx[0] < data->ao_cnt)
	{
		idx[1] = -1;
		while (++idx[1] < data->pipe_cnt[idx[0]])
		{
			info = (t_cmd_info *)(data->tkn[idx[0]][idx[1]]->head.contents);
			if (info->parentheses_flag == TRUE)
			{
				tmp_str = (char *)(data->tkn[idx[0]][idx[1]]
						->tail.front->contents);
				idx[2] = 0;
				while (tmp_str[++idx[2]] != '\0')
				{
					if (tmp_str[idx[2]] == '<' && tmp_str[idx[2] - 1] == '<')
						dll_node_move_to_another_dll_tail(tmp_h_dll->head.back,
							tmp_h_dll, &info->heredoc_names);
				}
			}
		}
	}
}
