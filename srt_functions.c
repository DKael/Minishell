/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srt_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:07:59 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 02:41:38 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int srt_compare(void *input_lst, int idx1, int idx2)
{
	t_srt *lst;
	int result;

	lst = (t_srt *)input_lst;
	result = ft_strcmp(lst[idx1].name, lst[idx2].name);
	if (result > 0)
		return (1);
	else if (result < 0)
		return (-1);
	else
		return (0);
}

void srt_swap(void *input_lst, int idx1, int idx2)
{
	t_srt *lst;
	t_srt temp;

	lst = (t_srt *)input_lst;
	temp = lst[idx1];
	lst[idx1] = lst[idx2];
	lst[idx2] = temp;
}
