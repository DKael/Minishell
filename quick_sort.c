/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:01:17 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/23 14:54:12 by junehyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quick_sort.h"

static void	quick_sort(t_srt_data data);
static int	make_partition(const t_srt_data *data);

void	sorting(void *lst, int size,
	int (*compare_func)(void *, int, int),
	void (*swap_func)(void *, int, int))
{
	t_srt_data	data;

	data.part = lst;
	data.left = 0;
	data.right = size - 1;
	data.compare_func = compare_func;
	data.swap_func = swap_func;
	quick_sort(data);
}

void	quick_sort(t_srt_data data)
{
	int			pivot;
	t_srt_data	left_srt_data;
	t_srt_data	right_srt_data;

	if (data.left < data.right)
	{
		pivot = make_partition(&data);
		left_srt_data = data;
		left_srt_data.right = pivot - 1;
		quick_sort(left_srt_data);
		right_srt_data = data;
		right_srt_data.left = pivot + 1;
		quick_sort(right_srt_data);
	}
}

int	make_partition(const t_srt_data *data)
{
	int	pivot;
	int	head;
	int	tail;

	pivot = data->left;
	head = data->left;
	tail = data->right + 1;
	while (head < tail)
	{
		while (++head < data->right
			&& data->compare_func(data->part, head, pivot) == -1)
			;
		while (--tail > data->left
			&& data->compare_func(data->part, tail, pivot) == 1)
			;
		if (head < tail)
			data->swap_func(data->part, head, tail);
	}
	data->swap_func(data->part, pivot, tail);
	return (tail);
}
