/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:20:18 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 16:04:28 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	total_heredoc_cnt_chk(char *cmd)
{
	int	idx;
	int	heredoc_cnt;

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

int	get_file_info(char *name, t_file_info *info, int mode)
{
	if (access(name, F_OK) == -1)
	{
		if (mode == 0)
			err_msg_print2(name, ": No such file or directory");
		return (1);
	}
	if (stat(name, &info->bf) == -1)
		return (4);
	info->mode = info->bf.st_mode;
	if (((info->bf.st_mode) & S_IFMT) == S_IFDIR)
		info->type = DIRECTORY;
	else if (((info->bf.st_mode) & S_IFMT) == S_IFREG)
		info->type = REGULAR_FILE;
	else if (((info->bf.st_mode) & S_IFMT) == S_IFLNK)
		info->type = SYMBOLIC_LINK;
	else
		info->type = ETC;
	return (0);
}
