/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list5.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:43:05 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 21:39:51 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

static void	dll_node_insert2(t_dllnode *node, t_dllnode *pos, int mode);

/*
If mode is 0, node is added at back of pos,
else, added in front of pos.
If pos is head, node is added at back of head despite of mode value.
If pos is tail, node is added in front of tail despite of mode value.
*/
t_bool	dll_node_insert(t_dllnode *node, t_dll *dll, t_dllnode *pos, int mode)
{
	if (dll_is_in(dll, pos) == FALSE)
		return (FALSE);
	if (pos == &(dll->head) || (mode == 1 && pos->front == &(dll->head)))
	{
		node->front = &(dll->head);
		node->back = dll->head.back;
		dll->head.back = node;
		node->back->front = node;
	}
	else if (pos == &(dll->tail) || (mode == 0 && pos->back == &(dll->tail)))
	{
		node->back = &(dll->tail);
		node->front = dll->tail.front;
		dll->tail.front = node;
		node->front->back = node;
	}
	else
		dll_node_insert2(node, pos, mode);
	dll->size++;
	return (TRUE);
}

static void	dll_node_insert2(t_dllnode *node, t_dllnode *pos, int mode)
{
	if (mode == 1)
		pos = pos->front;
	node->front = pos;
	node->back = pos->back;
	pos->back = node;
	node->back->front = node;
}

/*
If mode is 0, node which include content is added in front of tail,
else, add back of head.
*/
t_bool	dll_content_add(t_dll *dll, void *content, int mode)
{
	t_dllnode	*node_ptr;

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
	t_dllnode	*node_ptr;
	t_bool		result;

	node_ptr = dll_new_node(content);
	if (node_ptr == T_NULL)
		return (FALSE);
	if (ptr == T_NULL)
	{
		free(node_ptr);
		return (FALSE);
	}
	if (mode == 0)
		result = dll_add_back(dll, ptr, node_ptr);
	else
		result = dll_add_front(dll, ptr, node_ptr);
	if (result == FALSE)
		free(node_ptr);
	return (result);
}

void	dll_print(t_dll *dll, void (*print)(void *))
{
	t_dllnode	*node_ptr;

	if (dll != T_NULL)
	{
		node_ptr = dll->head.back;
		while (node_ptr != &(dll->tail))
		{
			if (print != T_NULL)
				print(node_ptr->contents);
			node_ptr = node_ptr->back;
		}
	}
}
