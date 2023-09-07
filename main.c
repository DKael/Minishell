/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/07 17:23:15 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void data_init(t_data *data)
{
	data->instr = T_NULL;
	data->instr_cnt = 0;
	data->pid_table = T_NULL;
	data->instr_infos = T_NULL;
}

t_bool check_quote(const char *instr)
{
	int idx;
	char	tmp;

	idx = -1;
	while (instr[++idx] != '\0')
	{
		if (instr[idx] == '\"' || instr[idx] == '\'')
		{
			tmp = instr[idx];
			while (instr[++idx] != tmp && instr[idx] != '\0')
				;
			if (instr[idx] == '\0')
				return (FALSE);
		}
	}
	return (TRUE);
}

int main(int argc, char **argv)
{
	t_data data;
	char *instr;
	int	idx;

	data.program_name = argv[0];
	if (argc != 1)
	{
		printf("%s don't support file read or need any other inputs\n", argv[0]);
		return (1);
	}
	data.wstatus = 0;
	while (1)
	{
		data_init(&data);
		instr = readline("minishell$ ");
		if (instr == T_NULL)
		{
			printf("exit\n");
			return (0);
		}
		else if (instr[0] == '\0')
		{
			free(instr);
			continue;
		}
		else
		{
			idx = -1;
			while (instr[++idx] != '\0')
			{
				if (instr[idx] == '\n')
				{
					printf("%s: syntax error, multiple lines input\n", data.program_name);
					free(instr);
					break;
				}
			}
			if (instr[idx] == '\n')
				continue;
			add_history(instr);
		}
			
		if (check_quote(instr) == FALSE)
		{
			printf("%s: syntax error, unclosed quote\n", data.program_name);
			free(instr);
			continue;
		}
		if (check_special_char_syntax(&instr) == FALSE)
		{
			data.wstatus = 258;
			free(instr);
			continue;
		}
		split_instr(&data, instr);
		// if (data.instr == T_NULL)
		// {
		// 	free(buffer);
		// 	return (1);
		// }
		free(instr);
		printf("syntax ok\n");

		// printf("\n\n----------------------------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------------------------\n\n");
	}
}
