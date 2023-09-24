/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:15:28 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 19:48:40 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

static t_bool	dll_del_node2(t_dllnode *node, void (*del)(void *));

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

t_bool	dll_del_node(t_dll *dll, t_dllnode *node, void (*del)(void *))
{
	if (dll != T_NULL && node != T_NULL && dll_is_in(dll, node) == TRUE)
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
		return (dll_del_node2(node, del));
	}
	return (FALSE);
}

static t_bool	dll_del_node2(t_dllnode *node, void (*del)(void *))
{
	if (del != T_NULL)
	{
		if (node->contents != T_NULL)
			del(node->contents);
		else
			free(node->contents);
	}
	free(node);
	return (TRUE);
}

t_bool	dll_node_move_to_head(t_dllnode *node, t_dll *dll)
{
	if (node != T_NULL && dll != T_NULL && dll_is_in(dll, node) == TRUE)
	{
		if (dll->size == 1 || node->front == &(dll->head))
			return (TRUE);
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
		dll_add_head(dll, node);
		return (TRUE);
	}
	return (FALSE);
}

t_bool	dll_node_move_to_tail(t_dllnode *node, t_dll *dll)
{
	if (node != T_NULL && dll != T_NULL && dll_is_in(dll, node) == TRUE)
	{
		if (dll->size == 1 || node->back == &(dll->tail))
			return (TRUE);
		else if (node->front == &(dll->head))
		{
			dll->head.back = node->back;
			node->back->front = &(dll->head);
		}
		else
		{
			node->front->back = node->back;
			node->back->front = node->front;
		}
		dll->size--;
		dll_add_tail(dll, node);
		return (TRUE);
	}
	return (FALSE);
}
