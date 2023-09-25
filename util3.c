/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:10:18 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:14:51 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_2d_array(void ***arr_ptr, int num)
{
	int		idx;
	void	**tmp;

	if (*arr_ptr != T_NULL)
	{
		tmp = *arr_ptr;
		idx = -1;
		while (++idx < num)
			ft_free1((void **)&tmp[idx]);
		ft_free1((void **)&tmp);
		*arr_ptr = T_NULL;
	}
	return (T_NULL);
}

void	*free_2d_array2(void ***arr_ptr)
{
	int		idx;
	void	**tmp;

	if (*arr_ptr != T_NULL)
	{
		tmp = *arr_ptr;
		idx = -1;
		while (tmp[++idx] != T_NULL)
			ft_free1((void **)&tmp[idx]);
		ft_free1((void **)&tmp);
		*arr_ptr = T_NULL;
	}
	return (T_NULL);
}

void	*ft_free1(void **ptr)
{
	free(*ptr);
	*ptr = T_NULL;
	return (T_NULL);
}

t_bool	ft_free2(void **ptr, t_bool flag)
{
	free(*ptr);
	*ptr = T_NULL;
	return (flag);
}

int	ft_free3(void **ptr, int return_num)
{
	free(*ptr);
	*ptr = T_NULL;
	return (return_num);
}
