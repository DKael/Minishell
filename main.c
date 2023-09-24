/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 00:12:13 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void store_env_in_dll(t_data *data, char **envp);
t_bool parentheses_heredoc(t_dll *heredoc_names, int *tkn_idx, char *cmd);
t_bool heredoc_split(t_dll *dll, char *tkns);
void set_last_exit_code(t_data *data, int input_exit_code);
void	check_oldpwd_exist(t_data *data);
static void builtin_func_error(t_data *data, int result);

void envdll_sorting(t_data *data);



void set_last_exit_code(t_data *data, int input_exit_code)
{
	int idx;
	int temp;

	input_exit_code %= 1000;
	data->last_exit_code = input_exit_code;
	temp = data->last_exit_code;
	idx = 0;
	if (temp / 100 != 0)
		data->last_exit_code_str[idx++] = (temp / 100) + '0';
	temp %= 100;
	if (temp / 10 != 0)
		data->last_exit_code_str[idx++] = (temp / 10) + '0';
	temp %= 10;
	data->last_exit_code_str[idx++] = temp + '0';
	data->last_exit_code_str[idx] = '\0';
}


void close_pipes(t_data *data, int num)
{
	int idx;

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

void resource_free_and_exit(t_data *data, int exit_code, char *msg)
{
	int idx;
	t_dllnode *ptr;

	if (msg != T_NULL)
		err_msg_print1(msg);
	ft_free1((void **)&data->cmd);
	free_2d_array((void ***)&data->ao_split, data->ao_cnt);
	idx = -1;
	while (++idx < data->ao_cnt)
		free_2d_dll(&data->tkn[idx], data->pipe_cnt[idx], str_delete_func);
	ft_free1((void **)&data->tkn);
	ft_free1((void **)&data->pipe_cnt);
	ft_free1((void **)&data->logic_table);
	dll_clear(&data->envdll, envval_delete_func);
	ptr = data->sorted_envdll.head.back;
	while (ptr != &(data->sorted_envdll.tail))
	{
		free(ptr);
		ptr = ptr->back;
	}
	opened_fd_close(data);
	exit(exit_code);
}

void on_execution_part_err(t_data *data, int pp_make_cnt, int exit_code, char *msg)
{
	close_pipes(data, pp_make_cnt);
	free_2d_array2((void ***)&data->pp);
	ft_free1((void **)&data->pid_table);
	resource_free_and_exit(data, exit_code, msg);
}



void total_heredoc_cnt_chk(char *cmd)
{
	int idx;
	int heredoc_cnt;

	heredoc_cnt = 0;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '<' && cmd[idx + 1] == '<')
		{
			heredoc_cnt++;
			if (heredoc_cnt > 16)
			{
				free(cmd);
				printf("minishell: maximum here-document count exceeded\n");
				exit(2);
			}
		}
	}
}

void convert_minus_value_to_whitespace(char *cmd)
{
	int idx;

	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == -1)
			cmd[idx] = ' ';
	}
}

int main(int argc, char **argv, char **envp)
{
	t_data data;

	int idx[2];
	int idx3;
	t_dllnode *ptr[2];
	t_cmd_info *info_ptr;
	char *tmp_str;
	t_bool result;
	t_dllnode *node_ptr;

	int ao_idx;
	int pp_idx;
	int node_idx;
	int int_tmp;
	int func_type;

	char **argu_lst;


	if (argc != 1)
	{
		printf("minishell: don't support file read or need any other inputs\n");
		return (1);
	}
	data_init(&data, argv[0], envp);
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
			ft_free1((void **)&data.cmd);
			continue;
		}
		add_history(data.cmd);
		if (check_syntax_error(&(data.cmd), 0) == FALSE)
		{
			set_last_exit_code(&data, 258);
			ft_free1((void **)&data.cmd);
			continue;
		}

		total_heredoc_cnt_chk(data.cmd);
		convert_minus_value_to_whitespace(data.cmd);
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
						resource_free_and_exit(&data, 1, "malloc error16");
					if (parentheses_heredoc(&info_ptr->heredoc_names, idx, tmp_str) == FALSE)
						resource_free_and_exit(&data, 1, "here document error");
					ft_free1((void **)&tmp_str);
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
						resource_free_and_exit(&data, 1, "here document error");
					ptr[0] = ptr[0]->back;
				}
			}
		}


		ao_idx = -1;
		while (++ao_idx < data.ao_cnt)
		{
			if (ao_idx > 0)
			{
				if ((data.logic_table[ao_idx - 1] == AND && data.last_exit_code != 0) || (data.logic_table[ao_idx - 1] == OR && data.last_exit_code == 0))
					continue;
			}

			pp_idx = -1;
			while (++pp_idx < data.pipe_cnt[ao_idx])
			{
				if (data.tkn[ao_idx][pp_idx]->size != 0)
					if (retrieve_variable_value(&data, data.tkn[ao_idx][pp_idx]) == FALSE)
						resource_free_and_exit(&data, 1, "malloc error17");
			}
			pp_idx = -1;
			while (++pp_idx < data.pipe_cnt[ao_idx])
			{
				if (data.tkn[ao_idx][pp_idx]->size != 0)
					result = wildcard(data.tkn[ao_idx][pp_idx]);
				if (result == -1)
					resource_free_and_exit(&data, 1, "malloc error");
				else if (result == -2)
					resource_free_and_exit(&data, 1, "opendir error");
			}
				

			if (data.pipe_cnt[ao_idx] == 1 && ((t_cmd_info *)(data.tkn[ao_idx][0]->head.contents))->cp_cnt > 0)
			{
				int_tmp = ((t_cmd_info *)(data.tkn[ao_idx][0]->head.contents))->redir_cnt;
				node_ptr = data.tkn[ao_idx][0]->head.back;
				node_idx = -1;
				while (++node_idx < int_tmp)
					node_ptr = node_ptr->back;
				func_type = is_builtin_func((char *)node_ptr->contents);
				if (func_type != 0)
				{
					if (int_tmp != 0 && (basic_redirection_save(&data) == FALSE))
						resource_free_and_exit(&data, 1, "redirection error");

					result = sign_redirection(&data, data.tkn[ao_idx][0]);
					if (result == 1)
					{
						set_last_exit_code(&data, 1);
						break;
					}
					else if (result == 2)
						resource_free_and_exit(&data, 1, "file open error");
					else if (result == 3)
						resource_free_and_exit(&data, 1, "redirection error");
					else if (result == 4)
						resource_free_and_exit(&data, 1, "stat() error");

					argu_lst = make_2d_array_from_dll(data.tkn[ao_idx][0]);
					if (argu_lst == T_NULL)
						resource_free_and_exit(&data, 1, "malloc error18");
					result = execute_builtin_func(func_type, argu_lst, &data);
					set_last_exit_code(&data, result);
					free(argu_lst);

					opened_fd_close(&data);
					if (int_tmp != 0 && basic_redirection_recover(&data) == FALSE)
						resource_free_and_exit(&data, 1, "redirection error");
					continue;
				}
			}

			if (data.pipe_cnt[ao_idx] > 1)
			{
				data.pp = (int **)ft_calloc(data.pipe_cnt[ao_idx] + 2, sizeof(int *));
				if (data.pp == T_NULL)
					resource_free_and_exit(&data, 1, "malloc error20");
				pp_idx = -1;
				while (++pp_idx < data.pipe_cnt[ao_idx] + 1)
				{
					data.pp[pp_idx] = (int *)ft_calloc(2, sizeof(int));
					if (data.pp[pp_idx] == T_NULL)
						on_execution_part_err(&data, 0, 1, "malloc error21");
				}
				pp_idx = 0;
				while (++pp_idx < data.pipe_cnt[ao_idx])
				{
					if (pipe(data.pp[pp_idx]) == -1)
						on_execution_part_err(&data, pp_idx, 1, "pipe error");
				}
				data.pp[0][0] = 0;
				data.pp[data.pipe_cnt[ao_idx]][1] = 1;
			}

			data.pid_table = (pid_t *)ft_calloc(data.pipe_cnt[ao_idx], sizeof(pid_t));
			if (data.pid_table == T_NULL)
				on_execution_part_err(&data, data.pipe_cnt[ao_idx], 1, "malloc error22");

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
					on_execution_part_err(&data, data.pipe_cnt[ao_idx], 1, "fork error");
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
			free_2d_array2((void ***)&data.pp);
			ft_free1((void **)&data.pid_table);
		}

		ft_free1((void **)&data.cmd);
		free_2d_array((void ***)&data.ao_split, data.ao_cnt);
		idx[0] = -1;
		while (++idx[0] < data.ao_cnt)
			free_2d_dll(&data.tkn[idx[0]], data.pipe_cnt[idx[0]], str_delete_func);
		ft_free1((void **)&data.tkn);
		ft_free1((void **)&data.pipe_cnt);
		ft_free1((void **)&data.logic_table);

		// printf("\n\n----------------------<7>----------------------\n\n");
		// system("leaks minishell");
		// printf("\n\n----------------------<7>----------------------\n\n");
	}
}
