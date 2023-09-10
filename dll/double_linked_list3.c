/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:15:28 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 02:25:13 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

void	dll_del_node(t_dll *dll, t_dllnode *node, void (*del)(void *))
{
	if (dll_is_in(dll, node) == TRUE)
	{
		if (dll->size-- == 1)
		{
			dll->head.back = &(dll->tail);
			dll->tail.front = &(dll->head);
		}
		else if (node->front == &(dll->head))
		{
			dll->head.back = node->back;
			node->back->front = &(dll->head);
		}
		else if (node->back == &(dll->tail))
		{
			dll->tail.front = node->front;
			node->front->back = &(dll->tail);
		}
		else
		{
			node->front->back = node->back;
			node->back->front = node->front;
		}
		del(node->contents);
		free(node);
	}
}

int	dll_node_compare(t_dllnode *n1, t_dllnode *n2,
	int (*compare)(t_dllnode *, t_dllnode *))
{
	int	result;

	result = compare(n1, n2);
	if (result == 1)
		return (1);
	else if (result == -1)
		return (-1);
	else
		return (0);
}

void	dll_node_move_to_another_dll_head(t_dllnode *node,
				t_dll *dll1, t_dll *dll2)
{
	if (node->front != T_NULL && node->back != T_NULL)
	{
		if (dll1->size == 1)
		{
			dll1->head.back = &(dll1->tail);
			dll1->tail.front = &(dll1->head);
		}
		else if (node->front == &(dll1->head))
		{
			dll1->head.back = node->back;
			node->back->front = &(dll1->head);
		}
		else if (node->back == &(dll1->tail))
		{
			dll1->tail.front = node->front;
			node->front->back = &(dll1->tail);
		}
		else
		{
			node->front->back = node->back;
			node->back->front = node->front;
		}
		dll1->size--;
		dll_add_head(dll2, node);
	}
}

void	dll_node_move_to_another_dll_tail(t_dllnode *node,
				t_dll *dll1, t_dll *dll2)
{
	if (node->front != T_NULL && node->back != T_NULL)
	{
		if (dll1->size == 1)
		{
			dll1->head.back = &(dll1->tail);
			dll1->tail.front = &(dll1->head);
		}
		else if (node->front == &(dll1->head))
		{
			dll1->head.back = node->back;
			node->back->front = &(dll1->head);
		}
		else if (node->back == &(dll1->tail))
		{
			dll1->tail.front = node->front;
			node->front->back = &(dll1->tail);
		}
		else
		{
			node->front->back = node->back;
			node->back->front = node->front;
		}
		dll1->size--;
		dll_add_tail(dll2, node);
	}
}
