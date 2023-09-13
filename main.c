/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/13 12:13:24 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void store_env_in_dll(t_data *data, char **envp);

void data_init(t_data *data, char *program_name, char **envp)
{
	data->program_name = program_name;
	data->envp = envp;
	data->last_exit_code = 0;
	dll_init(&data->envdll);
	store_env_in_dll(data, envp);
}

void data_cycle_init(t_data *data)
{
	data->tkn = T_NULL;
	data->ao_cnt = 0;
	data->pipe_cnt = T_NULL;
	data->pid_table = T_NULL;
	data->logic_table = T_NULL;
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
	int	idx;

	while (*envp)
	{
		idx = -1;
		while ((*envp)[++idx] != '=' && (*envp)[idx] != '\0')
			;
		env = (t_envval *)ft_calloc(1, sizeof(t_envval));
		if (env == T_NULL)
		{
			dll_clear(&data->envdll, envval_delete_func);
			message_exit("minishell: malloc error!\n", 1);
		}
		env->name = ft_strndup(*envp, idx);
		env->value = ft_strdup(&(*envp)[idx + 1]);
		if (env->name == T_NULL || env->value == T_NULL)
		{
			dll_clear(&data->envdll, envval_delete_func);
			envval_delete_func((void *)env);
			message_exit("minishell: malloc error!\n", 1);
		}
		if (dll_content_add(&data->envdll, (void *)env, 0) == FALSE)
		{
			dll_clear(&data->envdll, envval_delete_func);
			envval_delete_func((void *)env);
			message_exit("minishell: malloc error!\n", 1);
		}
		envp++;
	}
}

t_bool check_syntax_error(char *cmd, int mode)
{
	if (check_multiple_lines(cmd) == FALSE || check_quote_closed(cmd) == FALSE || check_parentheses_syntax(cmd) == FALSE || check_dollor_braces(cmd) == FALSE)
		return (FALSE);
	if (mode == 0)
		if ((check_special_char_syntax(&cmd) == FALSE))
			return (FALSE);
	return (TRUE);
}

void dll_env_print_func(void *content)
{
	t_envval *tmp;

	tmp = (t_envval *)content;
	printf("%s=%s\n", tmp->name, tmp->value);
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
	// printf("\n\n----------------------<1>----------------------\n\n");
	// system("leaks minishell");
	// printf("\n\n----------------------<1>----------------------\n\n");

	data_init(&data, argv[0], envp);

	// printf("\n\n----------------------<2>----------------------\n\n");
	// system("leaks minishell");
	// printf("\n\n----------------------<2>----------------------\n\n");
	dll_print(&data.envdll, dll_env_print_func);
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
		// printf("\n\n----------------------<3>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<3>----------------------\n\n");
		if (check_syntax_error(cmd, 0) == FALSE)
		{
			set_last_exit_code(&data, 258);
			free(cmd);
			continue;
		}
		// printf("\n\n----------------------<4>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<4>----------------------\n\n");
		split_cmd(&data, cmd);
		// printf("\n\n----------------------<5>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<5>----------------------\n\n");

		printf("\n------------------------------------\n");
		t_dllnode *ptr;
		for (int i = 0; i < data.ao_cnt; i++)
		{
			for (int j = 0; j < data.pipe_cnt[i]; j++)
			{
				ptr = data.tkn[i][j]->head.back;
				while (ptr != &data.tkn[i][j]->tail)
				{
					printf("%s\n", (char *)ptr->contents);
					ptr = ptr->back;
				}
			}
		}

		// printf("\n\n----------------------<6>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<6>----------------------\n\n");

		free(cmd);
		dll_clear(&data.envdll, envval_delete_func);
		free_2d_array(&data.ao_split, data.ao_cnt);
		idx = -1;
		while (++idx < data.ao_cnt)
			free_2d_dll(&data.tkn[idx], data.pipe_cnt[idx], str_delete_func);
		free(data.tkn);
		free(data.pipe_cnt);
		free(data.logic_table);
		printf("syntax ok\n\n");

		printf("\n\n----------------------<7>----------------------\n\n");
		system("leaks minishell");
		printf("\n\n----------------------<7>----------------------\n\n");
	}
}
