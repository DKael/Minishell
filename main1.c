/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/21 16:44:19 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void store_env_in_dll(t_data *data, char **envp);
t_bool parentheses_heredoc(t_dll *heredoc_names, int *tkn_idx, char *cmd);
t_bool heredoc_split(t_dll *dll, char *tkns);

void	envdll_sorting(t_data *data);

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
	data->pp = T_NULL;
	data->pid_table = T_NULL;
	data->logic_table = T_NULL;
}

void set_last_exit_code(t_data *data, int input_exit_code)
{
	int idx;
	int temp;

	data->last_exit_code = input_exit_code;
	temp = data->last_exit_code;
	idx = 0;
	if (temp / 100 != 0)
		data->last_exit_code_str[idx++] = (temp / 100);
	temp %= 100;
	if (temp / 10 != 0)
		data->last_exit_code_str[idx++] = (temp / 10);
	temp %= 10;
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
		if ((*envp)[idx] == '=')
		{
			env->name = ft_strndup(*envp, idx);
			env->value = ft_strdup(&(*envp)[idx + 1]);
		}
		else if ((*envp)[idx] == '\0')
		{
			env->name = ft_strdup(*envp);
			env->value = ft_strdup("");
		}
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
	envdll_sorting(data);
}

int	srt_compare(void *input_lst, int idx1, int idx2)
{
	t_srt	*lst;
	int		result;

	lst = (t_srt *)input_lst;
	result = ft_strcmp(lst[idx1].name, lst[idx2].name);
	if (result > 0)
		return (1);
	else if (result < 0)
		return (-1);
	else
		return (0);
}

void	srt_swap(void *input_lst, int idx1, int idx2)
{
	t_srt	*lst;
	t_srt	temp;

	lst = (t_srt *)input_lst;
	temp = lst[idx1];
	lst[idx1] = lst[idx2];
	lst[idx2] = temp;
}

void	envdll_sorting(t_data *data)
{
	t_srt *tmp;
	t_dllnode *node_ptr;
	int		idx;

	tmp = (t_srt *)ft_calloc(data->envdll.size, (sizeof(t_srt)));
	if (tmp == T_NULL)
	{
		dll_clear(&data->envdll, envval_delete_func);
		message_exit("minishell: malloc error!\n", 1);
	}
	idx = -1;
	node_ptr = data->envdll.head.back;
	while (++idx <  data->envdll.size)
	{
		tmp[idx].name = ((t_envval *)(node_ptr->contents))->name;
		tmp[idx].ptr = node_ptr;
		node_ptr = node_ptr->back;
	}
	sorting((void *)tmp, data->envdll.size, srt_compare, srt_swap);
	dll_init(&data->sorted_envdll);
	idx = -1;
	while (++idx <  data->envdll.size)
	{
		if (dll_content_add(&data->sorted_envdll, tmp[idx].ptr, 0) == FALSE)
		{
			dll_clear(&data->envdll, envval_delete_func);
			dll_clear(&data->sorted_envdll, envval_delete_func);
			message_exit("minishell: malloc error!\n", 1);
		}
	}
}

t_bool check_syntax_error(char **cmd, int mode)
{
	if (check_multiple_lines(*cmd) == FALSE || check_quote_closed(*cmd) == FALSE
		|| check_parentheses_syntax(*cmd) == FALSE || check_dollor_braces(*cmd) == FALSE)
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
	if (tmp != T_NULL && tmp->value[0] != '\0')
		printf("%s=%s\n", tmp->name, tmp->value);
}

void dll_export_print_func(void *content)
{
	t_dllnode *tmp1;
	t_envval *tmp2;

	tmp1 = (t_dllnode *)content;
	tmp2 = (t_envval *)(tmp1->contents);
	if (tmp2 != T_NULL)
	{
		if (tmp2->value[0] != '\0')
		printf("%s=%s\n", tmp2->name, tmp2->value);
		else
			printf("%s\n", tmp2->name);
	}
	
}

void dll_str_print_func(void *content)
{
	char *tmp;

	tmp = (char *)content;
	printf("%s\n", tmp);
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

void resource_free_and_exit(t_data *data)
{
	int idx;

	free(data->cmd);
	dll_clear(&data->envdll, envval_delete_func);
	free_2d_array((void ***)&data->ao_split, data->ao_cnt);
	idx = -1;
	while (++idx < data->ao_cnt)
		free_2d_dll(&data->tkn[idx], data->pipe_cnt[idx], str_delete_func);
	free(data->tkn);
	free(data->pipe_cnt);
	free(data->logic_table);
	free(data->pid_table);
	exit(1);
}

void	on_execution_part_error(t_data *data, int pp_make_cnt, int pp_cnt)
{
	close_pipes(data, pp_make_cnt);
	free_2d_array((void ***)&data->pp, pp_cnt);
	free(data->pid_table);
	resource_free_and_exit(data);
}
//unset--------------------------------------------------------------




int main(int argc, char **argv, char **envp)
{
	t_data data;
/*
	int idx[2];
	int idx3;
	t_dllnode *ptr[2];
	t_cmd_info *info_ptr;
	char *tmp_str;
	t_bool	result;

	int	ao_idx;
	int	pp_idx;
*/
	if (argc != 1)
	{
		printf("minishell: don't support file read or need any other inputs\n");
		return (1);
	}
	// t_envval test[3];
	// t_dllnode *tmp_node_ptr;

	// test[0].name = "a";
	// test[0].value = "a";
	// test[1].name = "b";
	// test[1].value = "b";
	// test[2].name = "c";
	// test[2].value = "c";

	
	/*
	t_envval *tmp;

	tmp = (t_envval *)ft_calloc(sizeof(t_envval), 1);
	tmp.name = fwefewf;
	tmp.value =fwefwe;
	dll_contents_add(&data.envdll, (void *)&tmp, 0);
	*/
	

	// dll_content_add(&data.envdll, &test[0], 0);
	// dll_content_add(&data.envdll, &test[1], 0);
	// dll_content_add(&data.envdll, &test[2], 0);


	// tmp_node_ptr = data.envdll.tail.front->front->front;
	// dll_content_add(&data.sorted_envdll, tmp_node_ptr, 0);
	// tmp_node_ptr = tmp_node_ptr->back;
	// dll_content_add(&data.sorted_envdll, tmp_node_ptr, 0);
	// tmp_node_ptr = tmp_node_ptr->back;
	// dll_content_add(&data.sorted_envdll, tmp_node_ptr, 0);

	// printf("envdll size : %d, sorted_envdll size : %d\n", data.envdll.size, data.sorted_envdll.size);
	data_init(&data, argv[0], envp);
	printf("\n------------------------env---------------------------\n");
	dll_print(&data.envdll, dll_env_print_func);

	printf("\n------------------------export---------------------------\n");
	dll_print(&data.sorted_envdll, dll_export_print_func);

	dll_del_node(&data.envdll, data.envdll.tail.front, envval_delete_func);

	printf("\n------------------------env---------------------------\n");
	dll_print(&data.envdll, dll_env_print_func);
//필요한부분 주석 해제해서 테스트 해보세요!!!!----------------------------------------------
/*
//env test----------------------------------------------------------------
	printf("\n------------------------env function test---------------------------\n");
	do_env(&data);
//unset test----------------------------------------------------------------	
//readline첫번째 줄은 unset이 있다고 가정하기 때문에 두번째 readline부터 우리가 원하는 기능이 된다 즉 2~4번째 readline인자만 지워진다	
	printf("\n------------------------unset env---------------------------\n");
	int	flag;
	char	**str;
	int	i = 0;

	str = malloc(sizeof(*str) * 5);
	str[4] = 0;
	while (i < 4)
	{
		str[i] = readline("check unset : ");
		i++;
	}
	flag = ft_unset(&data.envdll, str);
	dll_print(&data.envdll, dll_env_print_func);
	return (flag);

	printf("\n------------------------export env---------------------------\n");
	int	flag;
	char	**str;
	int	i = 0;

	str = malloc(sizeof(*str) * 5);
	str[4] = 0;
	while (i < 4)
	{
		str[i] = readline("check export : ");
		i++;
	}
	flag = ft_export(&data.envdll, str);
	dll_print(&data.envdll, dll_env_print_func);
	return (flag);
*/
//여기까지-----------------------------------------------------------------------------------

	// printf("\n------------------------export---------------------------\n");
	// dll_print(&data.sorted_envdll, dll_export_print_func);
/*
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
		if (check_syntax_error(&(data.cmd), 0) == FALSE)
		{
			set_last_exit_code(&data, 258);
			free(data.cmd);
			continue;
		}
		split_cmd(&data, data.cmd);

		
		idx[0] = -1;
		while (++idx[0] < data.ao_cnt)
		{
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
					free(tmp_str);
					ptr[1] = info_ptr->heredoc_names.head.back;
				}
				ptr[0] = data.tkn[idx[0]][idx[1]]->head.back;
				idx3 = -1;
				while (++idx3 < info_ptr->redir_cnt)
				{
					tmp_str = ft_strstr((char *)(ptr[0]->contents), "<<");
					if (tmp_str == T_NULL)
					{
						ptr[0] = ptr[0]->back;
						continue;
					}
					if (info_ptr->parentheses_flag == TRUE)
						result = heredoc_make1_2(&info_ptr->heredoc_names, ptr[1], idx, tmp_str + 3);
					else  
						result = heredoc_make1_1(&info_ptr->heredoc_names, idx, tmp_str + 3);
					if (result == FALSE)
						resource_free_and_exit(&data);
					ptr[0] = ptr[0]->back;
				}
			}
		}

		// Print result to check all code work fine. It will be deleted at the end.
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
				printf("\n\n");
			}
		}

		// execution part

		

		sem_unlink("print_sem");
		data.print_sem = sem_open("print_sem", O_EXCL | O_CREAT, 0644, 1);



		ao_idx = -1;
		while (++ao_idx < data.ao_cnt)
		{
			if (ao_idx > 0)
			{
				if ((data.logic_table[ao_idx - 1] == AND && data.last_exit_code != 0) || (data.logic_table[ao_idx - 1] == OR && data.last_exit_code == 0))
				{
					continue;
				}
			}
			data.pp = (int **)ft_calloc(data.pipe_cnt[ao_idx] + 1, sizeof(int *));
			if (data.pp == T_NULL)
				resource_free_and_exit(&data);
			pp_idx = -1;
			while (++pp_idx < data.pipe_cnt[ao_idx] + 1)
			{
				data.pp[pp_idx] = (int *)ft_calloc(2, sizeof(int));
				if (data.pp[pp_idx] == T_NULL)
					on_execution_part_error(&data, 0, pp_idx);
			}
			pp_idx = 0;
			while (++pp_idx < data.pipe_cnt[ao_idx])
			{
				if (pipe(data.pp[pp_idx]) == -1)
					on_execution_part_error(&data, pp_idx, data.pipe_cnt[ao_idx] + 1);
			}

			data.pid_table = (pid_t *)ft_calloc(data.pipe_cnt[ao_idx], sizeof(pid_t));
			if (data.pid_table == T_NULL)
				on_execution_part_error(&data, data.pipe_cnt[ao_idx], data.pipe_cnt[ao_idx] + 1);

			pp_idx = -1;
			while (++pp_idx < data.pipe_cnt[ao_idx])
			{
				data.pid_table[pp_idx] = fork();
				if (data.pid_table[pp_idx] == -1)
				{
					while (--pp_idx >= 0)
					{
						kill(data.pid_table[pp_idx], SIGTERM);
						wait(T_NULL);
					}
					on_execution_part_error(&data, data.pipe_cnt[ao_idx], data.pipe_cnt[ao_idx] + 1);
				}
				else if (data.pid_table[pp_idx] == 0)
				{
					child(&data, ao_idx, pp_idx);
					exit(0);
				}
					
			}
			close_pipes(&data, data.pipe_cnt[ao_idx]);
			pp_idx = -1;
			while (++pp_idx < data.pipe_cnt[ao_idx])
			{
				if (data.pid_table[data.pipe_cnt[ao_idx] - 1] == wait(&data.last_exit_code))
					set_last_exit_code(&data, (data.last_exit_code >> 8) & 0xFF);
			}
			close_pipes(&data, data.pipe_cnt[ao_idx]);
			free_2d_array((void ***)&data.pp, data.pipe_cnt[ao_idx] + 1);
			free(data.pid_table);
		}

		sem_close(data.print_sem);
		sem_unlink("print_sem");


		// printf("\n\n----------------------<6>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<6>----------------------\n\n");

		free(data.cmd);
		dll_clear(&data.envdll, envval_delete_func);
		free_2d_array((void ***)&data.ao_split, data.ao_cnt);
		idx[0] = -1;
		while (++idx[0] < data.ao_cnt)
			free_2d_dll(&data.tkn[idx[0]], data.pipe_cnt[idx[0]], str_delete_func);
		free(data.tkn);
		free(data.pipe_cnt);
		free(data.logic_table);
		printf("syntax ok\n\n");

		// printf("\n\n----------------------<7>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<7>----------------------\n\n");
	}
*/
}