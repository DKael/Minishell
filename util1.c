/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 17:43:07 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 17:43:01 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ignore_quote(char *cmd, int *idx)
{
	char	quote;

	quote = cmd[*idx];
	while (cmd[++(*idx)] != quote)
		;
}

void	ignore_parentheses(char *cmd, int *idx)
{
	int	layer;

	layer = 0;
	while (1)
	{
		(*idx)++;
		if (cmd[(*idx)] == ')')
		{
			if (layer == 0)
				break ;
			else
				layer--;
		}
		else if (cmd[(*idx)] == '(')
			layer++;
		else if (cmd[(*idx)] == '\'' || cmd[(*idx)] == '\"')
			ignore_quote(cmd, idx);
	}
}

void	set_exit_code(t_data *data, int input_exit_code)
{
	int	idx;
	int	temp;

	input_exit_code %= 1000;
	data->exit_code = input_exit_code;
	temp = input_exit_code;
	idx = 0;
	if (temp / 100 != 0)
		data->exit_code_str[idx++] = (temp / 100) + '0';
	temp %= 100;
	if (temp / 10 != 0)
		data->exit_code_str[idx++] = (temp / 10) + '0';
	temp %= 10;
	data->exit_code_str[idx++] = temp + '0';
	data->exit_code_str[idx] = '\0';
}

void	close_pipes(t_data *data, int num)
{
	int	idx;

	if (data->pp != T_NULL)
	{
		idx = 0;
		while (++idx < num)
		{
			if (data->pp[idx] != T_NULL)
			{
				close(data->pp[idx][0]);
				close(data->pp[idx][1]);
			}
		}
	}
}

void	convert_minus_value_to_whitespace(char *cmd)
{
	int	idx;

	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == -1)
			cmd[idx] = ' ';
	}
}
