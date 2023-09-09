/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:15:22 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/22 22:32:56 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

void	dll_init(t_dll *dll)
{
	dll->head.front = T_NULL;
	dll->head.back = &(dll->tail);
	dll->tail.front = &(dll->head);
	dll->tail.back = T_NULL;
	dll->size = 0;
}

void	dll_add_head(t_dll *dll, t_dllnode *new)
{
	new->front = &(dll->head);
	new->back = dll->head.back;
	dll->head.back->front = new;
	dll->head.back = new;
	dll->size++;
}

void	dll_add_tail(t_dll *dll, t_dllnode *new)
{
	new->front = dll->tail.front;
	new->back = &(dll->tail);
	dll->tail.front->back = new;
	dll->tail.front = new;
	dll->size++;
}

t_bool	dll_is_in(t_dll *dll, t_dllnode *check)
{
	t_dllnode	*next_node;

	next_node = dll->head.back;
	while (next_node != &(dll->tail))
	{
		if (next_node == check)
			return (TRUE);
	}
	return (FALSE);
}

t_dllnode	*dll_find(t_dll *dll, void *contents, t_bool (*f)(void *, void *))
{
	t_dllnode	*next_node;

	next_node = dll->head.back;
	while (next_node != &(dll->tail))
	{
		if (f(contents, next_node->contents) == TRUE)
			return (next_node);
		next_node = next_node->back;
	}
	return (T_NULL);
}
