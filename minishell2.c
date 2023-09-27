/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:33:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 21:54:47 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_make0_1(t_data *data, t_cmd_info *info,
				t_dllnode **ptr, int *idx);
static void	heredoc_make0_2(t_data *data, t_cmd_info *info,
				t_dllnode **ptr, int *idx);

void	heredoc_make0(t_data *data)
{
	int			idx[3];
	t_dllnode	*ptr[2];
	t_cmd_info	*info;

	idx[0] = -1;
	while (++idx[0] < data->ao_cnt)
	{
		idx[1] = -1;
		while (++idx[1] < data->pipe_cnt[idx[0]])
		{
			info = (t_cmd_info *)(data->tkn[idx[0]][idx[1]]->head.contents);
			heredoc_make0_1(data, info, ptr, idx);
			heredoc_make0_2(data, info, ptr, idx);
		}
	}
}

static void	heredoc_make0_1(t_data *data, t_cmd_info *info,
	t_dllnode **ptr, int *idx)
{
	char	*tmp_str;

	if (info->parentheses_flag == TRUE)
	{
		ptr[0] = data->tkn[idx[0]][idx[1]]->tail.front;
		tmp_str = ft_strdup((char *)(ptr[0]->contents));
		if (tmp_str == T_NULL)
			resource_free_and_exit(data, 1, "malloc error");
		if (parentheses_heredoc(&info->heredoc_names, idx, tmp_str) == FALSE)
			resource_free_and_exit(data, 1, "here document error");
		ft_free1((void **)&tmp_str);
		ptr[1] = info->heredoc_names.head.back;
	}
}

static void	heredoc_make0_2(t_data *data, t_cmd_info *info,
	t_dllnode **ptr, int *idx)
{
	char		*tmp_str;
	t_bool		result;

	ptr[0] = data->tkn[idx[0]][idx[1]]->head.back;
	idx[2] = -1;
	while (++idx[2] < info->redir_cnt)
	{
		tmp_str = ft_strstr((char *)(ptr[0]->contents), "<<");
		if (tmp_str == T_NULL)
		{
			ptr[0] = ptr[0]->back;
			continue ;
		}
		if (info->parentheses_flag == TRUE)
			result = heredoc_make1_2(&info->heredoc_names,
					ptr[1], idx, tmp_str + 3);
		else
			result = heredoc_make1_1(&info->heredoc_names, idx, tmp_str + 3);
		if (result == FALSE)
			resource_free_and_exit(data, 1, "here document error");
		ptr[0] = ptr[0]->back;
	}
}
