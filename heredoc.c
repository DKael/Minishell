/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 09:26:30 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/17 14:52:04 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		free(tmp[0]);
		free(tmp[1]);
		return (ft_free1(tmp[2]));
	}
	bf1 = ft_strjoin2(tmp[0], tmp[1], "_");
	bf2 = ft_strjoin2(tmp[2], "heredoc", ".");
	free(tmp[0]);
	free(tmp[1]);
	free(tmp[2]);
	return (make_name2(make_path, bf1, bf2));
}
static char	*make_name2(char *make_path, char *bf1, char *bf2)
{
	char	*tmp;
	char	*result;
	
	if (bf1 == T_NULL || bf2 == T_NULL)
	{
		free(bf1);
		return (ft_free1(bf2));
	}
	tmp = ft_strjoin2(bf1, bf2, "_");
	free(bf1);
	free(bf2);
	if (tmp == T_NULL)
		return (T_NULL);
	result = ft_strjoin2(make_path, tmp, "/");
	free(tmp);
	return (result);
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
		free(make_path);
		make_path = ft_strdup(".");
		if (make_path == T_NULL)
			return (FALSE);
	}
	name = make_name(make_path, idx[0], idx[1], dll->size);
	free(make_path);
	if (name == T_NULL)
		return (FALSE);
	if (heredoc_make2(name, del) == FALSE)
		return (ft_free2(name, FALSE));
	if (dll_content_add(dll, (void *)name, 0) == FALSE)
	{
		unlink(name);
		return (ft_free2(name, FALSE));
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
		free(make_path);
		make_path = ft_strdup(".");
		if (make_path == T_NULL)
			return (FALSE);
	}
	name = make_name(make_path, idx[0], idx[1], dll->size);
	free(make_path);
	if (name == T_NULL)
		return (FALSE);
	if (heredoc_make2(name, del) == FALSE)
		return (ft_free2(name, FALSE));
	if (dll_content_add2(dll, (void *)name, ptr, 1) == FALSE)
	{
		unlink(name);
		return (ft_free2(name, FALSE));
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
		free(buffer);
	}
	free(buffer);
	free(paste_nl);
	close(fd);
}
