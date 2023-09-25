/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:01:05 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 18:08:34 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_convert5(t_dll *dll, char **tmp, char *buffer, int *s_chk)
{
	if (tmp[2][0] == '*' && tmp[2][1] == '\0' && tmp[3][0] == '.')
		;
	else if (pattern_matching(tmp[2], tmp[3]) == TRUE)
	{
		if (tmp[1] == &buffer[0])
			tmp[0] = ft_strjoin2("/", &buffer[s_chk[1]], tmp[3]);
		else
		{
			buffer[s_chk[0]] = '/';
			buffer[s_chk[0] + 1] = '\0';
			tmp[0] = ft_strjoin2(tmp[1], &buffer[s_chk[1]], tmp[3]);
		}
		if (tmp[0] == T_NULL)
			return (ft_free3((void **)&tmp[2], -1));
		if (dll_content_add(dll, tmp[0], 0) == FALSE)
		{
			free(tmp[0]);
			return (ft_free3((void **)&tmp[2], -1));
		}
	}
	return (0);
}

int	do_convert6(t_dir *dir_data, char *buffer, char **tmp, int *s_chk)
{
	struct dirent	*file;

	file = readdir(dir_data->dir);
	while (file != T_NULL)
	{
		tmp[3] = file->d_name;
		if (do_convert5(dir_data->dll, tmp, buffer, s_chk) == -1)
		{
			closedir(dir_data->dir);
			return (-1);
		}
		file = readdir(dir_data->dir);
	}
	closedir(dir_data->dir);
	return (ft_free3((void **)&tmp[2], 0));
}

t_bool	pattern_matching(char *part, char *file)
{
	int		idx[2];

	idx[0] = ft_strlen(part);
	idx[1] = ft_strlen(file);
	if ((part[0] != '*' && part[0] != file[0])
		|| (part[idx[0] - 1] != '*' && part[idx[0] - 1] != file[idx[1] - 1]))
		return (FALSE);
	idx[0] = 0;
	idx[1] = 0;
	if (part[0] == '*')
		idx[1] = -1;
	while (part[++idx[0]] != '\0')
	{
		if ((part[idx[0]] != '*' && part[idx[0] - 1] != '*')
			&& (part[idx[0]] != file[++idx[1]]))
			return (FALSE);
		else if (part[idx[0]] != '*' && part[idx[0] - 1] == '*')
		{
			while (file[++idx[1]] != part[idx[0]] && file[idx[1]] != '\0')
				;
			if (file[idx[1]] == '\0')
				return (FALSE);
		}
	}
	return (TRUE);
}
