/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:05:32 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:28:58 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	str_delete_func(void *str)
{
	char	*temp;

	if (str != T_NULL)
	{
		temp = (char *)str;
		ft_free1((void **)&temp);
	}
}

void	envval_delete_func(void *envval)
{
	t_envval	*temp;

	if (envval != T_NULL)
	{
		temp = (t_envval *)envval;
		ft_free1((void **)&temp->name);
		ft_free1((void **)&temp->value);
		ft_free1((void **)&temp);
	}
}
