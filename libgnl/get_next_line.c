/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 16:46:12 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 19:50:55 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_fd_node	*start;
	t_gnldata			data;

	data.fd_start = &start;
	data.it = start;
	data.it_back = start;
	while (data.it != T_NULL && data.it->fd != fd)
	{
		data.it_back = data.it;
		data.it = data.it->next;
	}
	if (data.it == T_NULL)
	{
		data.it = (t_fd_node *)malloc(sizeof(t_fd_node));
		if (data.it == T_NULL)
			return ((T_NULL));
		data.it->fd = fd;
		data.it->start = 0;
		data.it->next = start;
		start = data.it;
		data.it_back = data.it;
	}
	data.node.size = 0;
	data.node.next = T_NULL;
	return (get_buffer_remains(&data));
}

char	*get_buffer_remains(t_gnldata *data)
{
	int			idx;
	char		*result;
	t_fd_node	*it;

	it = data->it;
	idx = it->start;
	if (idx != 0 && it->end > it->start)
	{
		while (it->bf[idx] != '\n' && idx < it->end)
			idx++;
		if (idx == BUFFER_SIZE)
		{
			data->node.size = idx - it->start;
			gnl_memmove(data->node.bf, &(it->bf[it->start]), data->node.size);
			result = read_lines1(data);
		}
		else
			result = case_nl_eof(data, idx, it);
	}
	else
		result = read_lines1(data);
	if (result == T_NULL)
		return (free_fd_and_node(data, CLEAN_ALL));
	return (result);
}

char	*case_nl_eof(t_gnldata *data, int idx, t_fd_node *it)
{
	char	*result;

	if (it->bf[idx] == '\n')
	{
		result = (char *)malloc(sizeof(char) * (idx - it->start + 2));
		if (result == T_NULL)
			return (T_NULL);
		result[idx - it->start + 1] = '\0';
		gnl_memmove(result, &(it->bf[it->start]), idx - it->start + 1);
		it->start = idx + 1;
		return (result);
	}
	else
	{
		result = (char *)malloc(sizeof(char) * (idx - it->start + 1));
		if (result == T_NULL)
			return (T_NULL);
		result[idx - it->start] = '\0';
		gnl_memmove(result, &(it->bf[it->start]), idx - it->start);
		free_fd_and_node(data, CLEAN_FD_ONLY);
		return (result);
	}
}

char	*read_lines1(t_gnldata *data)
{
	int			idx;
	t_lnode		*tmake;
	t_lnode		*pos;
	t_fd_node	*it;

	it = data->it;
	it->read = read(it->fd, it->bf, BUFFER_SIZE);
	if (it->read == -1 || (it->read == 0 && data->node.size == 0))
		return (T_NULL);
	pos = &(data->node);
	while (it->read == BUFFER_SIZE)
	{
		idx = 0;
		parse(data, &idx, &pos, &tmake);
		if (tmake == T_NULL)
			return (T_NULL);
		if (it->bf[idx] == '\n')
			return (do_concat(&(data->node)));
		it->read = read(it->fd, it->bf, BUFFER_SIZE);
		if (it->read == -1)
			return (T_NULL);
	}
	return (read_lines2(data, &pos));
}

char	*read_lines2(t_gnldata *data, t_lnode **pos)
{
	t_fd_node	*it;
	t_lnode		*tmake;
	int			idx;

	it = data->it;
	if (it->read == 0)
	{
		free_fd_and_node(data, CLEAN_FD_ONLY);
		return (do_concat(&(data->node)));
	}
	idx = 0;
	parse(data, &idx, pos, &tmake);
	if (tmake == T_NULL)
		return (T_NULL);
	if (it->bf[idx] != '\n' || (idx == it->read && it->bf[idx] == '\n'))
		free_fd_and_node(data, CLEAN_FD_ONLY);
	return (do_concat(&(data->node)));
}
