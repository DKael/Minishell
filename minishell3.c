/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:34:50 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 21:55:38 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	case_no_pipe_execute_builtin(t_data *data, int ao_idx, int *tmp);

int	check_next_ao_cmd(t_data *data, int ao_idx)
{
	if (ao_idx > 0)
	{
		if ((data->logic_table[ao_idx - 1] == AND && data->exit_code != 0)
			|| (data->logic_table[ao_idx - 1] == OR && data->exit_code == 0))
			return (1);
	}
	return (0);
}

void	dollor_sign_check(t_data *data, int ao_idx)
{
	int	pp_idx;

	pp_idx = -1;
	while (++pp_idx < data->pipe_cnt[ao_idx])
	{
		if (data->tkn[ao_idx][pp_idx]->size != 0)
			if (retrieve_variable_value(data,
					data->tkn[ao_idx][pp_idx]) == FALSE)
				resource_free_and_exit(data, 1, "malloc error");
	}
}

void	wildcard_check(t_data *data, int ao_idx)
{
	int	pp_idx;
	int	result;

	pp_idx = -1;
	while (++pp_idx < data->pipe_cnt[ao_idx])
	{
		if (data->tkn[ao_idx][pp_idx]->size != 0)
			result = wildcard(data->tkn[ao_idx][pp_idx]);
		if (result == -1)
			resource_free_and_exit(data, 1, "malloc error");
		else if (result == -2)
			resource_free_and_exit(data, 1, "opendir error");
		else if (result == -3)
			resource_free_and_exit(data, 1, "stat error");
	}
}

int	case_no_pipe_check_isbuiltin(t_data *data, int ao_idx)
{
	int			tmp[4];
	t_dllnode	*node_ptr;

	if (data->pipe_cnt[ao_idx] == 1
		&& ((t_cmd_info *)(data->tkn[ao_idx][0]->head.contents))->cp_cnt > 0)
	{
		tmp[0] = ((t_cmd_info *)
				(data->tkn[ao_idx][0]->head.contents))->redir_cnt;
		node_ptr = data->tkn[ao_idx][0]->head.back;
		tmp[1] = -1;
		while (++tmp[1] < tmp[0])
			node_ptr = node_ptr->back;
		tmp[2] = is_builtin_func((char *)node_ptr->contents);
		if (tmp[2] != 0)
			return (case_no_pipe_execute_builtin(data, ao_idx, tmp));
	}
	return (0);
}

static int	case_no_pipe_execute_builtin(t_data *data, int ao_idx, int *tmp)
{
	char		**argu_lst;

	if (tmp[0] != 0 && (basic_redirection_save(data) == FALSE))
		resource_free_and_exit(data, 1, "redirection error");
	tmp[3] = sign_redirection(data, data->tkn[ao_idx][0]);
	if (check_sign_redirection_result(data, tmp[3]) == 1)
		return (2);
	argu_lst = make_2d_array_from_dll(data->tkn[ao_idx][0]);
	if (argu_lst == T_NULL)
		resource_free_and_exit(data, 1, "malloc error");
	tmp[3] = execute_builtin_func(tmp[2], argu_lst, data);
	set_exit_code(data, tmp[3]);
	free(argu_lst);
	opened_fd_close(data);
	if (tmp[0] != 0 && basic_redirection_recover(data) == FALSE)
		resource_free_and_exit(data, 1, "redirection error");
	return (1);
}
