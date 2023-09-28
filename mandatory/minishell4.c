/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:36:41 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 21:56:30 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_pipe(t_data *data, int ao_idx)
{
	int	pp_idx;

	if (data->pipe_cnt[ao_idx] > 1)
	{
		data->pp = (int **)ft_calloc(data->pipe_cnt[ao_idx] + 2, sizeof(int *));
		if (data->pp == T_NULL)
			resource_free_and_exit(data, 1, "malloc error");
		pp_idx = -1;
		while (++pp_idx < data->pipe_cnt[ao_idx] + 1)
		{
			data->pp[pp_idx] = (int *)ft_calloc(2, sizeof(int));
			if (data->pp[pp_idx] == T_NULL)
				on_execution_part_err(data, 0, 1, "malloc error");
		}
		pp_idx = 0;
		while (++pp_idx < data->pipe_cnt[ao_idx])
		{
			if (pipe(data->pp[pp_idx]) == -1)
				on_execution_part_err(data, pp_idx, 1, "pipe error");
		}
		data->pp[0][0] = 0;
		data->pp[data->pipe_cnt[ao_idx]][1] = 1;
	}
}

void	make_child(t_data *data, int ao_idx)
{
	int	pp_idx;

	data->pid_table = (pid_t *)ft_calloc(data->pipe_cnt[ao_idx], sizeof(pid_t));
	if (data->pid_table == T_NULL)
		on_execution_part_err(data, data->pipe_cnt[ao_idx], 1, "malloc error");
	pp_idx = -1;
	while (++pp_idx < data->pipe_cnt[ao_idx])
	{
		data->pid_table[pp_idx] = fork();
		if (data->pid_table[pp_idx] == -1)
		{
			while (--pp_idx >= 0)
			{
				kill(data->pid_table[pp_idx], SIGTERM);
				wait(T_NULL);
			}
			on_execution_part_err(data, data->pipe_cnt[ao_idx],
				1, "fork error");
		}
		else if (data->pid_table[pp_idx] == 0)
		{
			child(data, ao_idx, pp_idx);
			exit(0);
		}
	}
}

void	wait_for_child(t_data *data, int ao_idx)
{
	int		pp_idx;
	pid_t	pid_tmp;

	pp_idx = -1;
	while (++pp_idx < data->pipe_cnt[ao_idx])
	{
		pid_tmp = waitpid(-1, &data->w_status, 0);
		if (ft_wifexited(data->w_status) == TRUE
			&& pid_tmp == data->pid_table[data->pipe_cnt[ao_idx] - 1])
			set_exit_code(data, ft_wexitstatus(data->w_status));
		else if (ft_wifsignaled(data->w_status) == TRUE)
		{
			if (ft_wtermsig(data->w_status) == SIGINT)
			{
				write(STDERR_FILENO, "\n", 1);
				set_exit_code(data, 130);
			}
			else if (ft_wtermsig(data->w_status) == SIGQUIT)
			{
				write(STDERR_FILENO, "Quit : 3\n", 9);
				set_exit_code(data, 131);
			}
		}
	}
}
