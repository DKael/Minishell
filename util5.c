/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 23:20:42 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 23:20:51 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	total_heredoc_cnt_chk(char *cmd)
{
	int	idx;
	int	heredoc_cnt;

	heredoc_cnt = 0;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '<' && cmd[idx + 1] == '<')
		{
			heredoc_cnt++;
			if (heredoc_cnt > 16)
			{
				free(cmd);
				printf("minishell: maximum here-document count exceeded\n");
				exit(2);
			}
		}
	}
}
