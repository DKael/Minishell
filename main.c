/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/04 20:54:04 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_init(t_data *data)
{
	data->instr = T_NULL;
	data->pipe_cnt = 0;
	data->wstatus = 0;
	data->pid_table = T_NULL;
}

int main(void)
{
	t_data	data;
	char *buffer;
	
	data_init(&data);
	while (1)
	{
		buffer = readline("minishell$ ");
		if (buffer == T_NULL)
		{
			printf("exit\n");
			return (0);
		}
		else if (buffer[0] == '\0')
		{
			data.wstatus = 1;
			free(buffer);
			continue;
		}
		if (check_special_char_syntax(buffer) == FALSE)
		{
			data.wstatus = 258;
			free(buffer);
			continue;
		}
		data.instr = parsing(&data, buffer);
		if (data.instr == T_NULL)
		{
			free(buffer);
			return (1);
		}
		free(buffer);
		printf("syntax ok\n");
	}
}