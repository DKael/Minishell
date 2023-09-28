/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:47:13 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 21:56:39 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	pipe_redirection(t_data *data, int ao_idx, int pp_idx)
{
	if (data->pp != T_NULL)
	{
		if (dup2(data->pp[pp_idx][0], STDIN_FILENO) != -1
			&& dup2(data->pp[pp_idx + 1][1], STDOUT_FILENO) != -1)
		{
			close_pipes(data, data->pipe_cnt[ao_idx]);
			return (TRUE);
		}
	}
	close_pipes(data, data->pipe_cnt[ao_idx]);
	return (FALSE);
}

t_bool	basic_redirection_save(t_data *data)
{
	if (dup2(STDIN_FILENO, data->old_stdin) == -1
		|| dup2(STDOUT_FILENO, data->old_stdout) == -1
		|| dup2(STDERR_FILENO, data->old_stderr) == -1)
		return (FALSE);
	else
		return (TRUE);
}

t_bool	basic_redirection_recover(t_data *data)
{
	if (dup2(data->old_stdin, STDIN_FILENO) == -1
		|| dup2(data->old_stdout, STDOUT_FILENO) == -1
		|| dup2(data->old_stderr, STDERR_FILENO) == -1)
		return (FALSE);
	return (TRUE);
}

t_bool	opened_fd_close(t_data *data)
{
	int		idx;
	t_bool	reset_flag;

	reset_flag = FALSE;
	idx = -1;
	while (++idx < MAX_FD)
	{
		if (reset_flag == FALSE
			&& (data->opened_fd[idx] == 0
				|| data->opened_fd[idx] == 1
				|| data->opened_fd[idx] == 2))
			reset_flag = TRUE;
		close(data->opened_fd[idx]);
		data->opened_fd[idx] = -1;
	}
	return (reset_flag);
}

int	check_sign_redirection_result(t_data *data, int result)
{
	if (result == 1)
	{
		set_exit_code(data, 1);
		return (1);
	}
	else if (result == 2)
		resource_free_and_exit(data, 1, "file open error");
	else if (result == 3)
		resource_free_and_exit(data, 1, "redirection error");
	else if (result == 4)
		resource_free_and_exit(data, 1, "stat() error");
	return (0);
}
