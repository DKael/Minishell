/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:20:18 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:59:31 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	resource_free_and_exit(t_data *data, int exit_code, char *msg)
{
	int			idx;
	t_dllnode	*ptr;

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
	heredoc_unlink(data);
	make_command_str();
	exit(exit_code);
}

void	on_execution_part_err(t_data *data, int pp_make_cnt,
	int exit_code, char *msg)
{
	close_pipes(data, pp_make_cnt);
	free_2d_array2((void ***)&data->pp);
	ft_free1((void **)&data->pid_table);
	resource_free_and_exit(data, exit_code, msg);
}

void	c_resource_free_and_exit(t_data *data, int exit_code, char *msg)
{
	int			idx;
	t_dllnode	*ptr;

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
	make_command_str();
	exit(exit_code);
}

void	child_free(t_cdata *cdata)
{
	free_2d_array2((void ***)&cdata->split_path);
	ft_free1((void **)&cdata->cmd_path);
	free(cdata->argu_lst);
	free(cdata->envp_lst);
}

void	child_exit(t_data *data, t_cdata *cdata,
	int exit_code, char *msg)
{
	close_pipes(data, cdata->pipe_cnt);
	child_free(cdata);
	free_2d_array2((void ***)&data->pp);
	ft_free1((void **)&data->pid_table);
	c_resource_free_and_exit(data, exit_code, msg);
}
