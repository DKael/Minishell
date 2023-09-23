/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 13:27:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 01:30:24 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pattern_matching(t_dll *dll, char *tkn);

int	wildcard(t_dll *dll)
{
	t_dll		tmp_dll;
	t_dllnode	*ptr;
	t_dllnode	*ptr1;
	t_dllnode	*ptr2;
	t_dllnode	*ptr3;
	t_dllnode	*del;
	t_bool		first_find;

	dll_init(&tmp_dll);
	first_find = FALSE;
	ptr = dll->head.back;
	ptr1 = T_NULL;
	ptr2 = T_NULL;
	while (ptr != &(dll->tail))
	{
		if (ft_strstr((char *)ptr->contents, "*") != T_NULL)
		{
			if (first_find == FALSE)
			{
				first_find = TRUE;
				ptr1 = ptr;
				ptr2 = ptr->back;
			}
			if (pattern_matching(&tmp_dll, (char *)ptr->contents) == -1)
				return (-1);
			ptr3 = tmp_dll.tail.front;
			while (ptr3 != &(tmp_dll.head))
			{
				ptr3 = ptr3->front;
				dll_node_insert(ptr3->back, dll, ptr, 0);
			}
			dll_init(&tmp_dll);
			if (ptr != ptr1)
			{
				del = ptr;
				ptr = ptr->front;
				dll_del_node(dll, ptr, str_delete_func);
			}
		}
		ptr = ptr->back;
		if (ptr == ptr2)
		{
			if (ptr->front != ptr1)
				dll_del_node(dll, ptr1, str_delete_func);
			first_find = FALSE;
		}
	}
	return (TRUE);
}

char *remove_duplicate_asterisk_slash(char *str)
{
	char *tmp;
	int idx1;
	int idx2;

	tmp = ft_strdup(str);
	if (tmp == T_NULL)
		return (T_NULL);
	idx1 = -1;
	idx2 = -1;
	while (str[++idx1] != '\0')
	{
		if (str[idx1] == '*' || str[idx1] == '/')
		{
			tmp[++idx2] = str[idx1];
			while (str[++idx1] == tmp[idx2])
				;
			idx1--;
		}
		else
			tmp[++idx2] = str[idx1];
	}
	tmp[++idx2] = '\0';
	return (tmp);
}

static int	pattern_matching(t_dll *dll, char *tkn)
{
	DIR		*dir;
	struct dirent *file;
	char	buffer[MAX_PATH_LEN];
	char	*path;
	char	*tmp2;
	int		idx1;
	int		idx2;
	t_bool	matching;

	if (tkn[0] != '/')
	{
		path = make_path(tkn, 1);
		if (path == T_NULL)
		{
			err_msg_print1("cd: malloc error");
			return (-1);
		}
		else if (path == (char *)-1)
			return (0);
		ft_strlcpy(buffer, path, MAX_PATH_LEN);
		free(path);
	}
	else
	{
		if (ft_strlcpy(buffer, tkn, MAX_PATH_LEN) >= MAX_PATH_LEN)
		{
			err_msg_print3("cd: ", tkn, ": File name too long");
			return (0);
		}
	}


	
	
	dir = opendir(tmp);
	if (dir == T_NULL)
	{
		free(tmp);
		return (-2);
	}
	free(tmp);
	tmp = (char *)ptr->contents;
	tmp2 = remove_duplicate_asterisk(tmp);
	if (tmp2 == T_NULL)
		return (-1);
	if (tmp2[0] == '*' && tmp2[1] == '\0')
	{
		ptr = ptr->back;
		file = readdir(dir);
		while (file != T_NULL)
		{
			tmp = ft_strdup(file->d_name);
			if (tmp == T_NULL)
			{
				free(tmp2);
				return (-1);
			}
			if (dll_content_add2(dll, (void *)tmp, ptr, 1) == FALSE)
			{
				free(tmp);
				free(tmp2);
				return (-1);
			}
			file = readdir(dir);
		}
		dll_del_node(dll, ptr, str_delete_func);
	}
	else
	{
		
		file = readdir(dir);
		while (file != T_NULL)
		{
			
			matching = TRUE;
			idx1 = ft_strlen(tmp);
			idx2 = ft_strlen(tmp2);
			if ((tmp[0] != '*' && tmp[0] != tmp2[0])
				|| (tmp[idx1 - 1] != '*' && tmp[idx1 - 1] != tmp2[idx2 - 1]))
				matching = FALSE;
			idx1 = -1;
			idx2 = -1;
			if (tmp[0] != '*')
			{
				idx1 = 0;
				idx2 = 0;
			}
			while (tmp[++idx1] != '\0' && matching == TRUE)
			{
				if (tmp[idx1] != '*')
				{
					
				}
			}
			if (matching == TRUE)
			{

			}
			free(tmp2);
			file = readdir(dir);
		}
	}
	free(tmp2);
	closedir(dir);
	return (0);
}