/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:23:45 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/10 16:31:49 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*gnl_memmove(void *dst, const void *src, size_t len)
{
	size_t			index;
	unsigned char	*dst_cptr;
	unsigned char	*src_cptr;

	index = -1;
	dst_cptr = (unsigned char *)dst;
	src_cptr = (unsigned char *)src;
	if (dst != T_NULL || src != T_NULL)
	{
		if (src - dst > 0)
		{
			while (++index < len)
				dst_cptr[index] = src_cptr[index];
		}
		else
		{
			while (++index < len)
				dst_cptr[len - index - 1] = src_cptr[len - index - 1];
		}
	}
	return (dst);
}

void	*gnl_lstclear(t_lnode **lst)
{
	t_lnode	*delete;
	t_lnode	*next_fd;

	if ((*lst) != T_NULL)
	{
		next_fd = (*lst);
		while (next_fd != T_NULL)
		{
			delete = next_fd;
			next_fd = next_fd->next;
			free(delete);
		}
		*lst = T_NULL;
	}
	return (T_NULL);
}

char	*do_concat(t_lnode *node_start)
{
	char	*result;
	int		len;
	t_lnode	*position;

	position = node_start;
	len = 0;
	while (position != T_NULL)
	{
		len += position->size;
		position = position->next;
	}
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (result == T_NULL)
		return (T_NULL);
	result[len] = '\0';
	position = node_start;
	len = 0;
	while (position != T_NULL)
	{
		gnl_memmove(&result[len], position->bf, position->size);
		len += position->size;
		position = position->next;
	}
	gnl_lstclear(&(node_start->next));
	return (result);
}

void	*free_fd_and_node(t_gnldata *data, int flag)
{
	if (data->it != T_NULL)
	{
		if (data->it != data->it_back)
			data->it_back->next = data->it->next;
		else
		{
			if (data->it->next != T_NULL)
				*(data->fd_start) = data->it->next;
			else
				*(data->fd_start) = T_NULL;
		}
		free(data->it);
		data->it = T_NULL;
	}
	if (flag == CLEAN_ALL)
		return (gnl_lstclear(&(data->node.next)));
	else
		return (T_NULL);
}

void	parse(t_gnldata *data, int *idx, t_lnode **pos, t_lnode **tmake)
{
	(*tmake) = (t_lnode *)malloc(sizeof(t_lnode));
	if ((*tmake) == T_NULL)
		return ;
	(*tmake)->next = T_NULL;
	(*pos)->next = (*tmake);
	(*pos) = (*tmake);
	while ((*idx) < data->it->read && data->it->bf[(*idx)] != '\n')
		(*idx)++;
	if (data->it->bf[(*idx)] == '\n' && (*idx) != data->it->read)
	{
		(*tmake)->size = (*idx) + 1;
		gnl_memmove((*tmake)->bf, data->it->bf, (*idx) + 1);
		if ((*idx) + 1 != data->it->read)
		{
			data->it->end = data->it->read;
			data->it->start = (*idx) + 1;
		}
		else
			data->it->start = 0;
		return ;
	}
	if ((*idx) == data->it->read)
		data->it->start = 0;
	(*tmake)->size = data->it->read;
	gnl_memmove((*tmake)->bf, data->it->bf, data->it->read);
}
