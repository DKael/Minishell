/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 13:27:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 17:44:34 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pattern_matching(t_dll *dll, char *tkn);
static t_bool pattern_matching2(char *part, char *file);

static int wildcard_redirection_check(t_dll *dll, t_dll *tmp_dll, t_dllnode **ptr)
{
	int			idx[2];
	char*		tkn;
	int			result;

	idx[0] = -1;
	while (++idx[0] < ((t_cmd_info *)(dll->head.contents))->redir_cnt)
	{
		tkn = (char *)(*ptr)->contents;
		if (ft_strstr(tkn, "*") != T_NULL && ft_strstr(tkn, "<<") == T_NULL)
		{
			tkn = ft_strstr(tkn, " ");
			result = pattern_matching(tmp_dll, ++tkn) == -1;
			if (result != 0)
				return (result);
			if (tmp_dll->size != 0)
			{
				err_msg_print2(&tkn[idx[1] + 1], ": ambiguous redirect");
				dll_clear(tmp_dll, str_delete_func);
				return (1);
			}
		}
		(*ptr) = (*ptr)->back;
	}
	return (0);
}

int	wildcard(t_dll *dll)
{
	
	t_dllnode	*ptr;
	t_dllnode	*ptr1;
	t_dllnode	*ptr2;
	t_dllnode	*ptr3;
	t_dllnode	*del;
	t_bool		first_find;
	t_dll		tmp_dll;
	int		result;
	

	dll_init(&tmp_dll);
	ptr = dll->head.back;
	result = wildcard_redirection_check(dll, &tmp_dll, &ptr);
	if (result != 0)
		return (result);
	first_find = FALSE;
	ptr1 = T_NULL;
	ptr2 = T_NULL;
	ptr = ptr->back;
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
			result = pattern_matching(&tmp_dll, (char *)ptr->contents);
			if (result == 2)
			{
				ptr = ptr->back;
				continue;
			}
			else if (result != 0)
			{
				dll_clear(&tmp_dll, str_delete_func);
				return (result);
			}
			((t_cmd_info *)(dll->head.contents))->cp_cnt += tmp_dll.size;
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
				dll_del_node(dll, del, str_delete_func);
				((t_cmd_info *)(dll->head.contents))->cp_cnt--;
			}
		}
		ptr = ptr->back;
		if (ptr == ptr2)
		{
			if (ptr->front != ptr1)
			{
				dll_del_node(dll, ptr1, str_delete_func);
				((t_cmd_info *)(dll->head.contents))->cp_cnt--;
			}
			first_find = FALSE;
		}
		// printf("\n\n---------------------------------------------\n");
		// dll_print(dll, dll_str_print_func);
		// printf("\n---------------------------------------------\n\n");
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
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;
	int		idx1;
	char	*part;
	int		s_chk[2];
	t_file_info f_info;

	tmp2 = remove_duplicate_asterisk_slash(tkn);
	if (tmp2 == T_NULL)
		return (-1);
	if (tmp2[0] != '/')
	{
		path = make_path(tmp2, 1);
		if (path == T_NULL)
		{
			err_msg_print1("malloc error");
			return (ft_free3((void **)&tmp2, -1));
		}
		else if (path == (char *)-1)
			return (ft_free3((void **)&tmp2, 0));
		else if (path == (char *)-2)
		{
			err_msg_print2(tkn, ": File name too long");
			return (ft_free3((void **)&tmp2, 0));
		}
		ft_strlcpy(buffer, path, MAX_PATH_LEN);
		free(path);
	}
	else
	{
		if (ft_strlcpy(buffer, tmp2, MAX_PATH_LEN) >= MAX_PATH_LEN)
		{
			err_msg_print2(tkn, ": File name too long");
			return (ft_free3((void **)&tmp2, 0));
		}
	}
	tmp3 = ft_strstr(buffer, tmp2);
	free(tmp2);

	idx1 = -1;
	while (buffer[++idx1] != '*')
		;
	s_chk[0] = idx1;
	while (buffer[--s_chk[0]] != '/')
		;
	s_chk[1] = idx1;
	while (buffer[++s_chk[1]] != '/' && buffer[s_chk[1]] != '\0')
		;
	if (buffer[s_chk[1]] == '/')
	{
		buffer[s_chk[1]] = '\0';
		part = ft_strdup(&buffer[s_chk[0] + 1]);
		buffer[s_chk[1]] = '/';
	}
	else
		part = ft_strdup(&buffer[s_chk[0] + 1]);
	if (part == T_NULL)
		return (-1);
		
	if (s_chk[0] == 0)
		buffer[++s_chk[0]] = '\0';
	else
		buffer[s_chk[0]] = '\0';
	
	if (get_file_info(buffer, &f_info, 1) == 4)
		return (-3);
	if (f_info.type != DIRECTORY)
		return (ft_free3((void **)&part, 0));
	
	if ((f_info.mode & 0700) != 0700)
		return (ft_free3((void **)&part, 2));

	dir = opendir(buffer);
	if (dir == T_NULL)
		return (ft_free3((void **)&part, -2));
		
	file = readdir(dir);
	while (file != T_NULL)
	{
		if (part[0] == '*' && part[1] == '\0' && file->d_name[0] == '.')
			;
		else if (pattern_matching2(part, file->d_name) == TRUE)
		{
			// if (s_chk[0] - 1 == 0)
			// 	tmp1 = ft_strjoin2("/", &buffer[s_chk[1]], file->d_name);
			// else
			// {
			// 	buffer[s_chk[0]] = '/';
			// 	buffer[s_chk[0] + 1] = '\0';
			// 	tmp1 = ft_strjoin2(buffer, &buffer[s_chk[1]], file->d_name);
			// }
			if (tmp3 == &buffer[0])
				tmp1 = ft_strjoin2("/", &buffer[s_chk[1]], file->d_name);
			else
			{
				buffer[s_chk[0]] = '/';
				buffer[s_chk[0] + 1] = '\0';
				tmp1 = ft_strjoin2(tmp3, &buffer[s_chk[1]], file->d_name);
			}
			if (tmp1 == T_NULL)
			{
				closedir(dir);
				return (ft_free3((void **)&part, -1));
			}
			if (dll_content_add(dll, tmp1, 0) == FALSE)
			{
				closedir(dir);
				free(part);
				return (ft_free3((void **)&part, -1));
			}
		}
		file = readdir(dir);
	}
	closedir(dir);
	return (ft_free3((void **)&part, 0));
}

static t_bool pattern_matching2(char *part, char *file)
{
	int		idx1;
	int		idx2;

	idx1 = ft_strlen(part);
	idx2 = ft_strlen(file);
	if ((part[0] != '*' && part[0] != file[0])
		|| (part[idx1 - 1] != '*' && part[idx1 - 1] != file[idx2 - 1]))
		return (FALSE);
	idx1 = 0;
	idx2 = 0;
	if (part[0] == '*')
		idx2 = -1;
	while (part[++idx1] != '\0')
	{
		if (part[idx1] != '*' && part[idx1 - 1] != '*')
		{
			if (part[idx1] != file[++idx2])
				return (FALSE);
		}
		else if (part[idx1] != '*' && part[idx1 - 1] == '*')
		{
			while (file[++idx2] != part[idx1] && file[idx2] != '\0')
				;
			if (file[idx2] == '\0')
				return (FALSE);
		}
	}
	return (TRUE);
}
