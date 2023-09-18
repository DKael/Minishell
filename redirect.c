/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:25:30 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/18 18:08:50 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool pipe_redirection(t_data *data, int ao_idx, int pp_idx)
{
	int	result;
	
	result = 0;
	result = dup2(data->pp[pp_idx][0], STDIN_FILENO);
	result = dup2(data->pp[pp_idx + 1][1], STDOUT_FILENO);
	close_pipes(data, data->pipe_cnt[ao_idx]);
	if (result == -1)
		return (FALSE);
	else
		return (TRUE);
}

t_bool	sign_redirection(t_dll *tkns)
{
	int	redir_cnt;
	int	redir_idx;
	t_dllnode *node_ptr;

	node_ptr = tkns->head.back;
	redir_cnt = ((t_cmd_info *)(tkns->head.contents))->redir_cnt;
	redir_idx = -1;
	while (++redir_idx < redir_cnt)
	{

		node_ptr = node_ptr->back;
	}
}

void	redirection_init()
{
	
}