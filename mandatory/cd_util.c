/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:51:57 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 21:14:45 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_chdir(char *path, t_data *data)
{
	if (chdir(path) == -1)
		return (-1);
	ft_strcpy(data->wd, path);
	return (0);
}

char	*remove_duplicate_slashs(char *str)
{
	char	*tmp;
	int		idx1;
	int		idx2;

	tmp = ft_strdup(str);
	if (tmp == T_NULL)
		return (T_NULL);
	idx1 = -1;
	idx2 = -1;
	while (str[++idx1] != '\0')
	{
		if (str[idx1] == '/')
		{
			tmp[++idx2] = str[idx1];
			while (str[++idx1] == '/')
				;
			idx1--;
		}
		else
			tmp[++idx2] = str[idx1];
	}
	tmp[++idx2] = '\0';
	return (tmp);
}

int	check_file(char *path)
{
	t_file_info	info;
	int			result;

	result = get_file_info(path, &info, 1);
	if (result != 0)
	{
		if (result == 1)
			err_msg_print3("cd: ", path, ": No such file or directory");
		return (result);
	}
	if (info.type == REGULAR_FILE)
	{
		err_msg_print3("cd: ", path, ": Not a directory");
		return (1);
	}
	if ((info.mode & 0700) != 0700)
	{
		err_msg_print3("cd: ", path, ": Permission denied");
		return (1);
	}
	return (0);
}
