/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/10 22:18:25 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void data_init(t_data *data, char *program_name, char **envp)
{
	data->program_name = program_name;
	data->envp = envp;
	data->instr = T_NULL;
	data->instr_cnt = 0;
	data->pid_table = T_NULL;
	data->instr_infos = T_NULL;
	data->wstatus = 0;
	dll_init(&data->envdll);
}

void data_cycle_init(t_data *data)
{
	data->instr = T_NULL;
	data->instr_cnt = 0;
	data->pid_table = T_NULL;
	data->instr_infos = T_NULL;
}

t_bool check_multiple_lines(const char *instr)
{
	int idx;

	idx = -1;
	while (instr[++idx] != '\0')
	{
		if (instr[idx] == '\n')
		{
			printf("minishell: syntax error, multiple lines input\n");
			return (FALSE);
		}
	}
	return (TRUE);
}

t_bool check_quote_closed(const char *instr)
{
	int idx;
	char tmp;

	idx = -1;
	while (instr[++idx] != '\0')
	{
		if (instr[idx] == '\"' || instr[idx] == '\'')
		{
			tmp = instr[idx];
			while (instr[++idx] != tmp && instr[idx] != '\0')
				;
			if (instr[idx] == '\0')
			{
				printf("minishell: syntax error, unclosed quote\n");
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

t_bool check_parentheses_syntax(char *instr)
{
	int idx;
	int save_idx;
	char char_tmp;
	t_bool empty_flag;
	int word_cnt;
	char *sliced_part;

	idx = -1;
	while (instr[++idx] != '\0')
	{
		if (instr[idx] == '(')
		{
			save_idx = idx;
			word_cnt = 0;
			while (--idx >= 0 && instr[idx] != '|' && instr[idx] != '&')
			{
				if (instr[idx] == '<' || instr[idx] == '>')
					return (syntax_error_print("("));
				else if (ft_isblank(instr[idx]) == FALSE)
				{
					if (ft_isblank(instr[idx]) == FALSE
					&& ft_isblank(instr[idx + 1]) == TRUE)
					{
						word_cnt++;
						if (word_cnt >= 2)
							return (syntax_error_print("("));
					}
					if (instr[idx] == '\"' || instr[idx] == '\'')
					{
						char_tmp = instr[idx];
						while (instr[--idx] != char_tmp)
							;
					}
				}
			}
			if (word_cnt == 1)
			{
				idx = save_idx;
				while (ft_isblank(instr[++idx]) == TRUE)
					;
				if (instr[idx] == ')')
				{
					printf("minishell: function not supported\n");
					return (FALSE);
				}
				else if (instr[idx] == '\0')
					return (syntax_error_print("newline"));
				save_idx = idx;
				while (instr[idx] != '\0' && instr[idx] != ')' && ft_isblank(instr[idx]) == FALSE)
				{
					if (instr[idx] == '\"' || instr[idx] == '\'')
					{
						char_tmp = instr[idx];
						while (instr[++idx] != char_tmp)
							;
					}
					idx++;
				}
					
				sliced_part = ft_strndup(&instr[save_idx], idx - save_idx);
				if (sliced_part == T_NULL)
				{
					printf("minishell: malloc error!\n");
					free(instr);
					exit(1);
				}
				syntax_error_print(sliced_part);
				free(sliced_part);
				return (FALSE);
			}

			empty_flag = TRUE;
			while (instr[++idx] != '\0')
			{
				if (instr[idx] == ')')
					break;
				else if (instr[idx] == '\"' || instr[idx] == '\'')
				{
					empty_flag = FALSE;
					char_tmp = instr[idx];
					while (instr[++idx] != char_tmp)
						;
				}
				else if (ft_isblank(instr[idx]) == FALSE)
					empty_flag = FALSE;
			}
			if (instr[idx] == '\0')
			{
				printf("minishell: syntax error, unclosed parentheses\n");
				return (FALSE);
			}
			else if (empty_flag == TRUE)
				return (syntax_error_print(")"));
		}
		else if (instr[idx] == '\"' || instr[idx] == '\'')
		{
			char_tmp = instr[idx];
			while (instr[++idx] != char_tmp)
				;
		}
		else if (instr[idx] == ')')
			return (syntax_error_print(")"));
	}
	return (TRUE);
}

int main(int argc, char **argv, char **envp)
{
	t_data data;
	char *instr;
	int idx;

	data_init(&data, argv[0], envp);
	if (argc != 1)
	{
		printf("minishell: don't support file read or need any other inputs\n");
		return (1);
	}
	while (1)
	{
		data_cycle_init(&data);
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
	
		add_history(instr);
		if (check_multiple_lines(instr) == FALSE
			|| check_quote_closed(instr) == FALSE
			|| check_parentheses_syntax(instr) == FALSE
			|| check_special_char_syntax(&instr) == FALSE)
		{
			data.wstatus = 258;
			free(instr);
			continue;
		}
		split_instr(&data, instr);
		for (int i = 0; i < data.instr_cnt; i++)
		{
			printf("\n<%d instruction set>\n", i + 1);
			for (int j = 0; j < data.instr_infos[i].size; j++)
			{
				printf("_%s_\n", data.instr[i][j]);
			}
		}
		free(instr);
		idx = -1;
		while (++idx < data.instr_cnt)
			free_2d_array(&data.instr[idx], data.instr_infos[idx].size);
		free(data.instr);
		free(data.instr_infos);

		printf("syntax ok\n\n");

		printf("\n\n----------------------------------------\n\n");
		system("leaks minishell");
		printf("\n\n----------------------------------------\n\n");
	}
}
