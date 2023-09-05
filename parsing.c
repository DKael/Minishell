/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/05 10:08:44 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char ***parsing(t_data *data, char *instr)
{
	// |  기준으로 나누기
	int	idx;
	int	pipe_cnt;

	idx = -1;
	while (instr[++idx] !='\0')
	{

	}
}

// pwd  -L      |      ||