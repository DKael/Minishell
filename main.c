/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/12 12:09:32 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void data_init(t_data *data, char *program_name, char **envp)
{
	data->program_name = program_name;
	data->envp = envp;
	data->last_exit_code = 0;
	dll_init(&data->envdll);
	store_env_in_dll(&data, envp);
}

void data_cycle_init(t_data *data)
{
	data->tkn = T_NULL;
	data->ao_cnt = 0;
	data->pipe_cnt = T_NULL;
	data->pid_table = T_NULL;
}


void set_last_exit_code(t_data *data, unsigned int input_exit_code)
{
	int idx;
	unsigned int temp;

	data->last_exit_code = input_exit_code;
	temp = data->last_exit_code;
	idx = 0;
	if (temp / 100U != 0)
		data->last_exit_code_str[idx++] = (temp / 100U);
	temp %= 100U;
	if (temp / 10U != 0)
		data->last_exit_code_str[idx++] = (temp / 10U);
	temp %= 10U;
	data->last_exit_code_str[idx++] = temp;
	data->last_exit_code_str[idx] = '\0';
}



void store_env_in_dll(t_data *data, char **envp)
{
	t_envval *env;
	char **temp;
	t_dllnode *node;

	while (*envp)
	{
		temp = ft_split(*envp, '=');
		if (temp == T_NULL)
		{
			dll_clear(&data->envdll, envval_delete_func);
			message_exit("minishell: malloc error!\n", 1);
		}
		env = (t_envval *)malloc(sizeof(t_envval));
		if (env == T_NULL)
		{
			dll_clear(&data->envdll, envval_delete_func);
			free_2d_array(&temp, 2);
			message_exit("minishell: malloc error!\n", 1);
		}
		env->name = temp[0];
		env->value = temp[1];
		node = dll_new_node((void *)env);
		if (node == T_NULL)
		{
			dll_clear(&data->envdll, envval_delete_func);
			free(env);
			free_2d_array(&temp, 2);
			message_exit("minishell: malloc error!\n", 1);
		}
		dll_add_tail(&data->envdll, node);
		free(temp);
		envp++;
	}
}

t_bool	check_syntax_error(char *cmd, int mode)
{
	if (check_multiple_lines(cmd) == FALSE
		|| check_quote_closed(cmd) == FALSE
		|| check_parentheses_syntax(cmd) == FALSE
		|| check_dollor_braces(cmd) == FALSE)
		return (FALSE);
	if (mode == 0)
		if ((check_special_char_syntax(&cmd) == FALSE))
			return (FALSE);
	return (TRUE);
}

int main(int argc, char **argv, char **envp)
{
	t_data data;
	char *cmd;
	int idx;

	if (argc != 1)
	{
		printf("minishell: don't support file read or need any other inputs\n");
		return (1);
	}
	data_init(&data, argv[0], envp);
	while (1)
	{
		data_cycle_init(&data);
		cmd = readline("minishell$ ");
		if (cmd == T_NULL)
		{
			printf("exit\n");
			return (0);
		}
		else if (cmd[0] == '\0')
		{
			free(cmd);
			continue;
		}

		add_history(cmd);
		if (check_syntax_error(cmd, 0) == FALSE)
		{
			set_last_exit_code(&data, 258);
			free(cmd);
			continue;
		}
		split_cmd(&data, cmd);



		

		printf("syntax ok\n\n");

		printf("\n\n----------------------------------------\n\n");
		system("leaks minishell");
		printf("\n\n----------------------------------------\n\n");
		
	}
}
