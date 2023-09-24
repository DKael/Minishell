/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:35:12 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 19:45:46 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

static void	dll_node_move_to_another_dll_head2(t_dllnode *node, t_dll *dll1);
static void	dll_node_move_to_another_dll_tail2(t_dllnode *node, t_dll *dll1);

void	dll_swap_node_contents(t_dllnode *node1, t_dllnode *node2)
{
	void	*temp;

	temp = node1->contents;
	node1->contents = node2->contents;
	node2->contents = temp;
}

t_bool	dll_node_move_to_another_dll_head(t_dllnode *node,
				t_dll *dll1, t_dll *dll2)
{
	if (node != T_NULL && dll1 != T_NULL && dll2 != T_NULL
		&&dll_is_in(dll1, node) == TRUE)
	{
		dll_node_move_to_another_dll_head2(node, dll1);
		dll1->size--;
		dll_add_head(dll2, node);
		return (TRUE);
	}
	return (FALSE);
}

static void	dll_node_move_to_another_dll_head2(t_dllnode *node, t_dll *dll1)
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
}

t_bool	dll_node_move_to_another_dll_tail(t_dllnode *node,
				t_dll *dll1, t_dll *dll2)
{
	if (node != T_NULL && dll1 != T_NULL && dll2 != T_NULL
		&&dll_is_in(dll1, node) == TRUE)
	{
		dll_node_move_to_another_dll_tail2(node, dll1);
		dll1->size--;
		dll_add_tail(dll2, node);
		return (TRUE);
	}
	return (FALSE);
}

static void	dll_node_move_to_another_dll_tail2(t_dllnode *node, t_dll *dll1)
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
}
