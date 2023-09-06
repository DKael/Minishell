/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/06 22:29:55 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_init(t_data *data)
{
	data->instr = T_NULL;
	data->pipe_cnt = 0;
	data->pid_table = T_NULL;
}

int main(void)
{
	t_data	data;
	char *buffer;
	
	data.wstatus = 0;
	while (1)
	{
		data_init(&data);
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
		if (check_special_char_syntax(&buffer) == FALSE)
		{
			data.wstatus = 258;
			free(buffer);
			continue;
		}
		data.instr = split_instr(&data, buffer);
		// if (data.instr == T_NULL)
		// {
		// 	free(buffer);
		// 	return (1);
		// }
		free(buffer);
		printf("syntax ok\n");

		// printf("\n\n----------------------------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------------------------\n\n");
	}
}

