/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:16:43 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 14:19:01 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	minishell2(t_data *data, int ao_idx);

int	minishell(char **argv, char **envp)
{
	t_data	data;
	int		ao_idx;

	data_init(&data, argv[0], envp);
	while (1)
	{
		if (read_cmd_and_syntax_check(&data) == 1)
			continue ;
		heredoc_make0(&data);
		signal(SIGINT, SIG_IGN);
		ao_idx = -1;
		while (++ao_idx < data.ao_cnt)
		{
			if (check_next_ao_cmd(&data, ao_idx) == 1)
				continue ;
			minishell2(&data, ao_idx);
		}
		end_one_cycle(&data);
	}
}

// printf("\n\n----------------------<leak check>----------------------\n\n");
// system("leaks minishell");
// printf("\n\n----------------------<leak check>----------------------\n\n");

static void	minishell2(t_data *data, int ao_idx)
{
	dollor_sign_check(data, ao_idx);
	wildcard_check(data, ao_idx);
	if (case_no_pipe_check_isbuiltin(data, ao_idx) == 1)
		return ;
	make_pipe(data, ao_idx);
	make_child(data, ao_idx);
	close_pipes(data, data->pipe_cnt[ao_idx]);
	wait_for_child(data, ao_idx);
	free_2d_array2((void ***)&data->pp);
	ft_free1((void **)&data->pid_table);
}

int	read_cmd(t_data *data)
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

int	read_cmd_and_syntax_check(t_data *data)
{
	if (read_cmd(data) == 1)
		return (1);
	if (check_syntax_error(&(data->cmd), 0) == FALSE)
	{
		set_exit_code(data, 258);
		ft_free1((void **)&data->cmd);
		return (1);
	}
	total_heredoc_cnt_chk(data->cmd);
	convert_minus_value_to_whitespace(data->cmd);
	split_cmd(data, data->cmd);
	return (0);
}
