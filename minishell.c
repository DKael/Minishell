/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:16:43 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 18:10:45 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	read_cmd(t_data *data)
{
	data_cycle_init(data);
	data->cmd = readline("minishell$ ");
	if (data->cmd == T_NULL)
	{
		printf("\033[1A");
		printf("\033[10C");
		printf(" exit\n");
		exit(data->exit_code % 256);
	}
	else if (data->cmd[0] == '\0')
	{
		ft_free1((void **)&data->cmd);
		return (1);
	}
	add_history(data->cmd);
	return (0);
}

int	minishell(char **argv, char **envp)
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
	pid_t	pid_tmp;

	data_init(&data, argv[0], envp);
	while (1)
	{
		if (read_cmd(&data) == 1)
			continue;
		if (check_syntax_error(&(data.cmd), 0) == FALSE)
		{
			set_exit_code(&data, 258);
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
						resource_free_and_exit(&data, 1, "malloc error");
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

		signal(SIGINT, SIG_IGN);
		ao_idx = -1;
		while (++ao_idx < data.ao_cnt)
		{
			if (ao_idx > 0)
			{
				if ((data.logic_table[ao_idx - 1] == AND && data.exit_code != 0) || (data.logic_table[ao_idx - 1] == OR && data.exit_code == 0))
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
				else if (result == -3)
					resource_free_and_exit(&data, 1, "stat error");
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
						set_exit_code(&data, 1);
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
					set_exit_code(&data, result);
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
				pid_tmp = waitpid(-1, &data.w_status, 0);
				if (ft_wifexited(data.w_status) == TRUE && pid_tmp == data.pid_table[data.pipe_cnt[ao_idx] - 1])
					set_exit_code(&data, ft_wexitstatus(data.w_status));
				else if (ft_wifsignaled(data.w_status) == TRUE)
				{
					if (ft_wtermsig(data.w_status) == SIGINT)
					{
						write(STDERR_FILENO, "\n", 1);
						set_exit_code(&data, 130);
					}
					else if (ft_wtermsig(data.w_status) == SIGQUIT)
					{
						write(STDERR_FILENO, "Quit : 3\n", 9);
						set_exit_code(&data, 131);
					}
				}
			}
			free_2d_array2((void ***)&data.pp);
			ft_free1((void **)&data.pid_table);
		}

		ft_free1((void **)&data.cmd);
		free_2d_array((void ***)&data.ao_split, data.ao_cnt);
		heredoc_unlink(&data);
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
