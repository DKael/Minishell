/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard3_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:00:41 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 17:35:44 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int		do_convert2(char *buffer, char *tkn, char *tmp);
static int		do_convert2_1(char *buffer, char *tkn, char *tmp);
static char		*do_convert3(char *buffer, int *s_chk);
static int		do_convert4(char *buffer, int *s_chk, t_dir	*dir_data,
					char *part);
extern int		do_convert5(t_dll *dll, char **tmp, char *buffer, int *s_chk);
extern int		do_convert6(t_dir *dir_data, char *buffer, char **tmp,
					int *s_chk);
extern t_bool	pattern_matching(char *part, char *file);
extern char		*remove_duplicate_asterisk_slash(char *str);

int	do_convert(t_dll *dll, char *tkn)
{
	t_dir	dir_data;
	char	buffer[MAX_PATH_LEN];
	char	*tmp[4];
	int		s_chk[3];

	dir_data.dll = dll;
	tmp[0] = remove_duplicate_asterisk_slash(tkn);
	if (tmp[0] == T_NULL)
		return (-1);
	s_chk[2] = do_convert2(buffer, tkn, tmp[0]);
	if (s_chk[2] == 1)
		return (0);
	else if (s_chk[2] != 0)
		return (s_chk[2]);
	tmp[1] = ft_strstr(buffer, tmp[0]);
	free(tmp[0]);
	tmp[2] = do_convert3(buffer, s_chk);
	if (tmp[2] == T_NULL)
		return (-1);
	s_chk[2] = do_convert4(buffer, s_chk, &dir_data, tmp[2]);
	if (s_chk[2] != 1)
		return (s_chk[2]);
	return (do_convert6(&dir_data, buffer, tmp, s_chk));
}

static int	do_convert2(char *buffer, char *tkn, char *tmp)
{
	if (tmp[0] != '/')
		return (do_convert2_1(buffer, tkn, tmp));
	else
	{
		if (ft_strlcpy(buffer, tmp, MAX_PATH_LEN) >= MAX_PATH_LEN)
		{
			err_msg_print2(tkn, ": File name too long");
			return (ft_free3((void **)&tmp, 1));
		}
	}
	return (0);
}

static int	do_convert2_1(char *buffer, char *tkn, char *tmp)
{
	char	*path;

	path = make_path(tmp, 1);
	if (path == T_NULL)
	{
		err_msg_print1("malloc error");
		return (ft_free3((void **)&tmp, -1));
	}
	else if (path == (char *)-1)
		return (ft_free3((void **)&tmp, 1));
	else if (path == (char *)-2)
	{
		err_msg_print2(tkn, ": File name too long");
		return (ft_free3((void **)&tmp, 1));
	}
	ft_strlcpy(buffer, path, MAX_PATH_LEN);
	free(path);
	return (0);
}

static char	*do_convert3(char *buffer, int *s_chk)
{
	int		idx;
	char	*part;

	idx = -1;
	while (buffer[++idx] != '*')
		;
	s_chk[0] = idx;
	while (buffer[--s_chk[0]] != '/')
		;
	s_chk[1] = idx;
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
	return (part);
}

static int	do_convert4(char *buffer, int *s_chk, t_dir	*dir_data, char *part)
{
	t_file_info	f_info;

	if (s_chk[0] == 0)
		buffer[++s_chk[0]] = '\0';
	else
		buffer[s_chk[0]] = '\0';
	if (get_file_info(buffer, &f_info, 1) == 4)
		return (ft_free3((void **)&part, -3));
	if (f_info.type != DIRECTORY)
		return (ft_free3((void **)&part, 0));
	if ((f_info.mode & 0700) != 0700)
		return (ft_free3((void **)&part, 2));
	dir_data->dir = opendir(buffer);
	if (dir_data->dir == T_NULL)
		return (ft_free3((void **)&part, 0));
	return (1);
}
