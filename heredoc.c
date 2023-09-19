/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 09:26:30 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/19 18:15:47 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
t_bool heredoc_split(t_dll *dll, char *tkns);
static char	*make_name2(char *make_path, char *bf1, char *bf2);
t_bool	heredoc_make2(char *name, char *del);
void	heredoc_make3(int fd, char *paste_nl);

char	*make_name(char *make_path, int idx1, int idx2, int size)
{
	char	*tmp[3];
	char	*bf1;
	char	*bf2;

	tmp[0] = ft_itoa(idx1);
	tmp[1] = ft_itoa(idx2);
	tmp[2] = ft_itoa(size);
	if (tmp[0] == T_NULL || tmp[1] == T_NULL || tmp[2] == T_NULL)
	{
		ft_free1((void **)&tmp[0]);
		ft_free1((void **)&tmp[1]);
		return (ft_free1((void **)&tmp[2]));
	}
	bf1 = ft_strjoin2(tmp[0], tmp[1], "_");
	bf2 = ft_strjoin2(tmp[2], "heredoc", ".");
	ft_free1((void **)&tmp[0]);
	ft_free1((void **)&tmp[1]);
	ft_free1((void **)&tmp[2]);
	return (make_name2(make_path, bf1, bf2));
}
static char	*make_name2(char *make_path, char *bf1, char *bf2)
{
	char	*tmp;
	char	*result;
	
	if (bf1 == T_NULL || bf2 == T_NULL)
	{
		ft_free1((void **)&bf1);
		return (ft_free1((void **)&bf2));
	}
	tmp = ft_strjoin2(bf1, bf2, "_");
	ft_free1((void **)&bf1);
	ft_free1((void **)&bf2);
	if (tmp == T_NULL)
		return (T_NULL);
	result = ft_strjoin2(make_path, tmp, "/");
	ft_free1((void **)&tmp);
	return (result);
}

t_bool parentheses_heredoc(t_dll *heredoc_names, int *tkn_idx, char *cmd)
{
	int idx;
	int p_idx[2];
	char *tmp;
	t_dllnode *ptr[2];
	t_dll tmp_dll;

	cmd[0] = ' ';
	cmd[ft_strlen(cmd) - 1] = ' ';
	p_idx[0] = -1;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '(')
		{
			p_idx[0] = idx;
			ignore_parentheses(cmd, &idx);
			p_idx[1] = idx;
		}
	}
	if (p_idx[0] != -1)
	{
		tmp = ft_strndup(&cmd[p_idx[0]], p_idx[1] - p_idx[0] + 1);
		if (tmp == T_NULL)
			return (FALSE);
		if (parentheses_heredoc(heredoc_names, tkn_idx, tmp) == FALSE)
			return (ft_free2((void **)&tmp, FALSE));
		ft_free1((void **)&tmp);
	}
	dll_init(&tmp_dll);
	if (heredoc_split(&tmp_dll, cmd) == FALSE)
		return (FALSE);
	ptr[1] = heredoc_names->head.back;
	ptr[0] = tmp_dll.head.back;
	while (ptr[0] != &(tmp_dll.tail))
	{
		tmp = ft_strstr((char *)(ptr[0]->contents), "<<");
		if (tmp == T_NULL)
		{
			ptr[0] = ptr[0]->back;
			continue;
		}
		if (heredoc_make1_2(heredoc_names, ptr[1], tkn_idx, tmp + 3) == FALSE)
		{
			dll_clear(&tmp_dll, str_delete_func);
			return (FALSE);
		}
		ptr[0] = ptr[0]->back;
	}
	dll_clear(&tmp_dll, str_delete_func);
	return (TRUE);
}

t_bool heredoc_split(t_dll *dll, char *tkns)
{
	int idx;
	int pos[4];
	char *con;

	idx = -1;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '<' && tkns[idx + 1] == '<')
		{
			find_front(tkns, pos, idx);
			find_back_and_calc_blank_quote(tkns, pos, idx);
			con = (char *)ft_calloc(pos[1] - pos[0] - pos[2] - pos[3] + 2, sizeof(char));
			if (con == T_NULL)
				return (FALSE);
			redirect_split2_1(tkns, con, pos, TRUE);
			if (dll_content_add(dll, (void *)con, 0) == FALSE)
				return (ft_free2((void **)&con, FALSE));
			idx = pos[1] - 1;
		}
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
	return (TRUE);
}

// heredoc name will be {idx1}_{idx2}_{dll's size}.heredoc
t_bool	heredoc_make1_1(t_dll *dll, int *idx, char *del)
{
	t_stat bf;
	char	*make_path;
	char	*name;
	
	make_path = ft_strdup(hd_path);
	if (make_path == T_NULL)
		return (FALSE);
	if (stat(hd_path, &bf) == -1 || !(((bf.st_mode) & S_IFMT) == S_IFDIR && bf.st_mode == 041777))
	{
		ft_free1((void **)&make_path);
		make_path = ft_strdup(".");
		if (make_path == T_NULL)
			return (FALSE);
	}
	name = make_name(make_path, idx[0], idx[1], dll->size);
	ft_free1((void **)&make_path);
	if (name == T_NULL)
		return (FALSE);
	if (heredoc_make2(name, del) == FALSE)
		return (ft_free2((void **)&name, FALSE));
	if (dll_content_add(dll, (void *)name, 0) == FALSE)
	{
		unlink(name);
		return (ft_free2((void **)&name, FALSE));
	}
	return (TRUE);
}
t_bool	heredoc_make1_2(t_dll *dll, t_dllnode *ptr, int *idx, char *del)
{
	t_stat bf;
	char	*make_path;
	char	*name;
	
	make_path = ft_strdup(hd_path);
	if (make_path == T_NULL)
		return (FALSE);
	if (stat(hd_path, &bf) == -1 || !(((bf.st_mode) & S_IFMT) == S_IFDIR && bf.st_mode == 041777))
	{
		ft_free1((void **)&make_path);
		make_path = ft_strdup(".");
		if (make_path == T_NULL)
			return (FALSE);
	}
	name = make_name(make_path, idx[0], idx[1], dll->size);
	ft_free1((void **)&make_path);
	if (name == T_NULL)
		return (FALSE);
	if (heredoc_make2(name, del) == FALSE)
		return (ft_free2((void **)&name, FALSE));
	if (dll_content_add2(dll, (void *)name, ptr, 1) == FALSE)
	{
		unlink(name);
		return (ft_free2((void **)&name, FALSE));
	}
	return (TRUE);
}

t_bool	heredoc_make2(char *name, char *del)
{
	int		fd;
	char	*paste_nl;

	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (FALSE);
	paste_nl = ft_strjoin(del, "\n");
	if (paste_nl == T_NULL)
	{
		unlink(name);
		return (FALSE);
	}
	heredoc_make3(fd, paste_nl);
	return (TRUE);
}

void	heredoc_make3(int fd, char *paste_nl)
{
	char	*buffer;

	while (1)
	{
		write(1, "> ", 3);
		buffer = get_next_line(0);
		if (buffer == T_NULL
			|| ft_strcmp(buffer, paste_nl) == 0)
			break ;
		write(fd, buffer, ft_strlen(buffer));
		ft_free1((void **)&buffer);
	}
	ft_free1((void **)&buffer);
	ft_free1((void **)&paste_nl);
	close(fd);
}
