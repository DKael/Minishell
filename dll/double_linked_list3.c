/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:15:28 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 16:20:18 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

static t_bool	dll_del_node2(t_dllnode *node, void (*del)(void *));
static void	dll_node_move_to_another_dll_head2(t_dllnode *node, t_dll *dll1);
static void	dll_node_move_to_another_dll_tail2(t_dllnode *node, t_dll *dll1);
static void	dll_node_insert2(t_dllnode *node, t_dllnode *pos, int mode);

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
