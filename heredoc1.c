/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 09:26:30 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 14:08:39 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char		*make_name(char *make_path, int idx1, int idx2, int size);
static t_bool	heredoc_make1_1_2(t_dll *dll, int *idx, char *del,
					char *make_path);
static t_bool	heredoc_make1_2_2(t_dll *dll, int *idx, char *del,
					char *make_path);
extern t_bool	heredoc_make2(char *name, char *del);
extern void		heredoc_make3(int fd, char *paste_nl);

t_bool	heredoc_make1_1(t_dll *dll, int *idx, char *del)
{
	t_stat	bf;
	char	*make_path;

	make_path = ft_strdup(HD_PATH);
	if (make_path == T_NULL)
		return (FALSE);
	if (stat(HD_PATH, &bf) == -1 || !(((bf.st_mode) & S_IFMT) == S_IFDIR
			&& bf.st_mode == 041777))
	{
		ft_free1((void **)&make_path);
		make_path = getcwd(0, 0);
		if (make_path == T_NULL)
			return (FALSE);
	}
	if (heredoc_make1_1_1(dll, idx, del, make_path) == FALSE)
		return (FALSE);
	return (TRUE);
}

static t_bool	heredoc_make1_1_2(t_dll *dll, int *idx, char *del,
	char *make_path)
{
	char	*name;

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
	t_stat	bf;
	char	*make_path;
	char	*name;

	make_path = ft_strdup(HD_PATH);
	if (make_path == T_NULL)
		return (FALSE);
	if (stat(HD_PATH, &bf) == -1 || !(((bf.st_mode) & S_IFMT) == S_IFDIR
			&& bf.st_mode == 041777))
	{
		ft_free1((void **)&make_path);
		make_path = ft_strdup(".");
		if (make_path == T_NULL)
			return (FALSE);
	}
	if (heredoc_make1_2_2(dll, idx, del, make_path) == FALSE)
		return (FALSE);
	if (dll_content_add2(dll, (void *)name, ptr, 1) == FALSE)
	{
		unlink(name);
		return (ft_free2((void **)&name, FALSE));
	}
	return (TRUE);
}

static t_bool	heredoc_make1_2_2(t_dll *dll, int *idx, char *del,
	char *make_path)
{
	char	*name;

	name = make_name(make_path, idx[0], idx[1], dll->size);
	ft_free1((void **)&make_path);
	if (name == T_NULL)
		return (FALSE);
	if (heredoc_make2(name, del) == FALSE)
		return (ft_free2((void **)&name, FALSE));
	return (TRUE);
}
