/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:15:28 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/16 16:29:14 by hyungdki         ###   ########.fr       */
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

/*
If mode is 0, node which include content add in front of tail,
else, add back of head.
*/
t_bool	dll_content_add(t_dll *dll, void *content, int mode)
{
	t_dllnode *node_ptr;

	node_ptr = dll_new_node(content);
	if (node_ptr == T_NULL)
		return (FALSE);
	if (mode == 0)
		dll_add_tail(dll, node_ptr);
	else
		dll_add_head(dll, node_ptr);
	return (TRUE);
}

/*
If mode is 0, node which include content add in back of ptr's node,
else, add front of ptr's node.
*/
t_bool	dll_content_add2(t_dll *dll, void *content, t_dllnode *ptr, int mode)
{
	t_dllnode *node_ptr;

	node_ptr = dll_new_node(content);
	if (node_ptr == T_NULL)
		return (FALSE);
	if (ptr == T_NULL)
		return (FALSE);
	if (mode == 0)
		dll_add_back(dll, ptr, node_ptr);
	else
		dll_add_front(dll, ptr, node_ptr);
	return (TRUE);
}

void	dll_print(t_dll *dll, void (*print)(void *))
{
	t_dllnode	*node_ptr;

	node_ptr = dll->head.back;
	while (node_ptr != &(dll->tail))
	{
		if (print != T_NULL)
			print(node_ptr->contents);
		node_ptr = node_ptr->back;
	}
}