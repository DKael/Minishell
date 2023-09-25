/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:10:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:15:27 by hyungdki         ###   ########.fr       */
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
