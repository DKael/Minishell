/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/16 17:14:26 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void store_env_in_dll(t_data *data, char **envp);
t_bool parentheses_heredoc(t_dll *heredoc_names, int *tkn_idx, char *cmd);
t_bool heredoc_split(t_dll *dll, char *tkns);

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
	data->cmd = T_NULL;
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
	int idx;

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

t_bool check_syntax_error(char **cmd, int mode)
{
	if (check_multiple_lines(*cmd) == FALSE || check_quote_closed(*cmd) == FALSE || check_parentheses_syntax(*cmd) == FALSE || check_dollor_braces(*cmd) == FALSE)
		return (FALSE);
	if (mode == 0)
		if ((check_special_char_syntax(cmd) == FALSE))
			return (FALSE);
	return (TRUE);
}

void dll_env_print_func(void *content)
{
	t_envval *tmp;

	tmp = (t_envval *)content;
	printf("%s=%s\n", tmp->name, tmp->value);
}

void dll_str_print_func(void *content)
{
	char *tmp;

	tmp = (char *)content;
	printf("%s\n", tmp);
}

void resource_free_and_exit(t_data *data)
{
	int idx;

	free(data->cmd);
	dll_clear(&data->envdll, envval_delete_func);
	free_2d_array(&data->ao_split, data->ao_cnt);
	idx = -1;
	while (++idx < data->ao_cnt)
		free_2d_dll(&data->tkn[idx], data->pipe_cnt[idx], str_delete_func);
	free(data->tkn);
	free(data->pipe_cnt);
	free(data->logic_table);
	exit(1);
}

int main(int argc, char **argv, char **envp)
{
	t_data data;
	int idx[2];
	t_dllnode *ptr[2];
	int idx3;
	t_cmd_info *info_ptr;
	char *tmp_str;

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
		data.cmd = readline("minishell$ ");
		if (data.cmd == T_NULL)
		{
			printf("exit\n");
			return (0);
		}
		else if (data.cmd[0] == '\0')
		{
			free(data.cmd);
			continue;
		}

		add_history(data.cmd);
		// printf("\n\n----------------------<3>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<3>----------------------\n\n");
		if (check_syntax_error(&(data.cmd), 0) == FALSE)
		{
			set_last_exit_code(&data, 258);
			free(data.cmd);
			continue;
		}
		// printf("\n\n----------------------<4>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<4>----------------------\n\n");
		split_cmd(&data, data.cmd);
		// printf("\n\n----------------------<5>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<5>----------------------\n\n");

		idx[0] = -1;
		while (++idx[0] < data.ao_cnt)
		{
			idx3 = -1;
			idx[1] = -1;
			while (++idx[1] < data.pipe_cnt[idx[0]])
			{
				info_ptr = (t_cmd_info *)(data.tkn[idx[0]][idx[1]]->head.contents);
				if (info_ptr->parentheses_flag == TRUE)
				{
					ptr[0] = data.tkn[idx[0]][idx[1]]->tail.front;
					tmp_str = ft_strdup((char *)(ptr[0]->contents));
					if (tmp_str == T_NULL)
						resource_free_and_exit(&data);
					if (parentheses_heredoc(&info_ptr->heredoc_names, idx, tmp_str) == FALSE)
						resource_free_and_exit(&data);
					ptr[0] = data.tkn[idx[0]][idx[1]]->head.back;
					ptr[1] = info_ptr->heredoc_names.head.back;
					while (++idx3 < info_ptr->redir_cnt)
					{
						tmp_str = ft_strstr((char *)(ptr[0]->contents), "<<");
						if (tmp_str != T_NULL && heredoc_make1_2(&info_ptr->heredoc_names, ptr[1], idx, tmp_str + 3) == FALSE)
							resource_free_and_exit(&data);
						ptr[0] = ptr[0]->back;
					}
				}
				else
				{
					ptr[0] = data.tkn[idx[0]][idx[1]]->head.back;
					while (++idx3 < info_ptr->redir_cnt)
					{
						tmp_str = ft_strstr((char *)(ptr[0]->contents), "<<");
						if (tmp_str != T_NULL && heredoc_make1_1(&info_ptr->heredoc_names, idx, tmp_str + 3) == FALSE)
							resource_free_and_exit(&data);
						ptr[0] = ptr[0]->back;
					}
				}
			}
		}

		printf("\n------------------------------------\n");

		for (int i = 0; i < data.ao_cnt; i++)
		{
			for (int j = 0; j < data.pipe_cnt[i]; j++)
			{
				printf("<heredoc %d_%d>\n", i, j);
				dll_print(&((t_cmd_info *)(data.tkn[i][j]->head.contents))->heredoc_names, dll_str_print_func);
				printf("------------------------\n");
				ptr[0] = data.tkn[i][j]->head.back;
				while (ptr[0] != &data.tkn[i][j]->tail)
				{
					printf("%s\n", (char *)ptr[0]->contents);
					ptr[0] = ptr[0]->back;
				}
			}
		}

		// printf("\n\n----------------------<6>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<6>----------------------\n\n");

		free(data.cmd);
		dll_clear(&data.envdll, envval_delete_func);
		free_2d_array(&data.ao_split, data.ao_cnt);
		idx[0] = -1;
		while (++idx[0] < data.ao_cnt)
			free_2d_dll(&data.tkn[idx[0]], data.pipe_cnt[idx[0]], str_delete_func);
		free(data.tkn);
		free(data.pipe_cnt);
		free(data.logic_table);
		printf("syntax ok\n\n");

		printf("\n\n----------------------<7>----------------------\n\n");
		system("leaks minishell");
		printf("\n\n----------------------<7>----------------------\n\n");
	}
}

t_bool heredoc_split(t_dll *dll, char *tkns)
{
	int idx;
	int pos[4];
	char *con;

	idx = -1;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '<' && tkns[idx + 1] == '<')
		{
			find_front(tkns, pos, idx);
			find_back_and_calc_blank_quote(tkns, pos, idx);
			con = (char *)ft_calloc(pos[1] - pos[0] - pos[2] - pos[3] + 2, sizeof(char));
			if (con == T_NULL)
				return (FALSE);
			redirect_split2_1(tkns, con, &pos[0], &pos[1]);
			if (dll_content_add(dll, (void *)con, 0) == FALSE)
				return (ft_free2((void *)con, FALSE));
			idx = pos[1] - 1;
		}
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
	return (TRUE);
}

t_bool parentheses_heredoc(t_dll *heredoc_names, int *tkn_idx, char *cmd)
{
	int idx;
	int p_idx[2];
	char *tmp;
	t_dllnode *ptr[2];
	t_dll tmp_dll;

	cmd[0] = ' ';
	cmd[ft_strlen(cmd) - 1] = ' ';
	p_idx[0] = -1;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '(')
		{
			p_idx[0] = idx;
			ignore_parentheses(cmd, &idx);
			p_idx[1] = idx;
		}
	}
	if (p_idx[0] != -1)
	{
		tmp = ft_strndup(&cmd[p_idx[0]], p_idx[1] - p_idx[0] + 1);
		if (tmp == T_NULL)
			return (FALSE);
		if (parentheses_heredoc(heredoc_names, tkn_idx, tmp) == FALSE)
			return (ft_free2(tmp, FALSE));
		free(tmp);
	}
	dll_init(&tmp_dll);
	if (heredoc_split(&tmp_dll, cmd) == FALSE)
		return (FALSE);
	ptr[1] = heredoc_names->head.back;
	ptr[0] = tmp_dll.head.back;
	while (ptr[0] != &(tmp_dll.tail))
	{
		tmp = ft_strstr((char *)(ptr[0]->contents), "<<");
		if (ptr[0] != T_NULL && heredoc_make1_2(heredoc_names, ptr[1], tkn_idx, tmp + 3) == FALSE)
		{
			dll_clear(&tmp_dll, str_delete_func);
			return (FALSE);
		}
		ptr[0] = ptr[0]->back;
	}
	dll_clear(&tmp_dll, str_delete_func);
	return (TRUE);
}