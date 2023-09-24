/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 17:43:07 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 02:41:46 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
