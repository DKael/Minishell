/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:15:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 19:43:22 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "double_linked_list.h"

t_bool	dll_add_front(t_dll *dll, t_dllnode *node, t_dllnode *new)
{
	if (dll_is_in(dll, node) == TRUE)
	{
		if (node->front == &(dll->head))
		{
			new->front = &(dll->head);
			new->back = node;
			dll->head.back = new;
			node->front = new;
		}
		else
		{
			node->front->back = new;
			new->front = node->front;
			new->back = node;
			node->front = new;
		}
		dll->size++;
		return (TRUE);
	}
	return (FALSE);
}

t_bool	dll_add_back(t_dll *dll, t_dllnode *node, t_dllnode *new)
{
	if (dll_is_in(dll, node) == TRUE)
	{
		if (node->back == &(dll->tail))
		{
			new->back = &(dll->tail);
			new->front = node;
			dll->tail.front = new;
			node->back = new;
		}
		else
		{
			node->back->front = new;
			new->front = node;
			new->back = node->back;
			node->back = new;
		}
		dll->size++;
		return (TRUE);
	}
	return (FALSE);
}

void	dll_clear(t_dll *dll, void (*del)(void *))
{
	t_dllnode	*delete;
	t_dllnode	*next_node;

	if (dll != T_NULL)
	{
		next_node = dll->head.back;
		while (next_node != &(dll->tail))
		{
			delete = next_node;
			next_node = next_node->back;
			if (delete->contents != T_NULL)
			{
				if (del != T_NULL)
					del(delete->contents);
				else
					free(delete->contents);
			}
			free(delete);
		}
		dll_init(dll);
	}
}

void	dll_ref_clear(t_dll *dll)
{
	t_dllnode	*delete;
	t_dllnode	*next_node;

	if (dll != T_NULL)
	{
		next_node = dll->head.back;
		while (next_node != &(dll->tail))
		{
			delete = next_node;
			next_node = next_node->back;
			free(delete);
		}
		dll_init(dll);
	}
}

t_dllnode	*dll_new_node(void *contents)
{
	t_dllnode	*result;

	result = (t_dllnode *)malloc(sizeof(t_dllnode));
	if (result == T_NULL)
		return (T_NULL);
	result->contents = contents;
	result->front = T_NULL;
	result->back = T_NULL;
	return (result);
}
