/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:10:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 21:57:32 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_2d_dll(t_dll ***dll_ptr, int num, void (*del)(void *))
{
	int		idx;
	t_dll	**tmp;

	if (*dll_ptr != T_NULL)
	{
		tmp = *dll_ptr;
		idx = -1;
		while (++idx < num)
		{
			if (tmp[idx] != T_NULL)
			{
				dll_clear(&((t_cmd_info *)
						(tmp[idx]->head.contents))->heredoc_names, del);
				ft_free1((void **)&tmp[idx]->head.contents);
				dll_clear(tmp[idx], del);
				ft_free1((void **)&tmp[idx]);
			}
		}
		ft_free1((void **)&tmp);
		*dll_ptr = T_NULL;
	}
	return (T_NULL);
}

void	end_one_cycle(t_data *data)
{
	int	idx;

	ft_free1((void **)&data->cmd);
	free_2d_array((void ***)&data->ao_split, data->ao_cnt);
	heredoc_unlink(data);
	idx = -1;
	while (++idx < data->ao_cnt)
		free_2d_dll(&data->tkn[idx], data->pipe_cnt[idx], str_delete_func);
	ft_free1((void **)&data->tkn);
	ft_free1((void **)&data->pipe_cnt);
	ft_free1((void **)&data->logic_table);
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
