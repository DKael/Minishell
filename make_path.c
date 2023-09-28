/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 20:19:24 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 20:22:15 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_path2(char *buffer, char **split, int bf_idx, int mode);
static void	make_path2_1(char *buffer, int bf_idx);
static char	*make_path2_2(char **split, int bf_idx, int idx, int mode);

char	*make_path(char *raw_path, int mode)
{
	char	buffer[MAX_PATH_LEN];
	char	**split;
	int		bf_idx;

	if (getcwd(buffer, MAX_PATH_LEN) == T_NULL)
	{
		if (mode == 0)
			write(2, "cd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 108);
		return ((char *)-1);
	}
	bf_idx = -1;
	while (buffer[++bf_idx] != '\0')
		;
	split = ft_split(raw_path, '/');
	if (split == T_NULL)
		return (T_NULL);
	return (make_path2(buffer, split, bf_idx, mode));
}

static char	*make_path2(char *buffer, char **split, int bf_idx, int mode)
{
	int	idx;
	int	idx2;

	idx = -1;
	while (split[++idx] != T_NULL)
	{
		if (split[idx][0] == '.' && split[idx][1] == '.'
			&& split[idx][2] == '\0')
			make_path2_1(buffer, bf_idx);
		else if ((split[idx][0] == '.' && split[idx][1] == '\0')
			|| split[idx][0] == '\0')
			continue ;
		else
		{
			if (make_path2_2(split, bf_idx, idx, mode) == T_NULL)
				return ((char *)-2);
			buffer[bf_idx] = '/';
			idx2 = -1;
			while (split[idx][++idx2] != '\0')
				buffer[++bf_idx] = split[idx][idx2];
			buffer[++bf_idx] = '\0';
		}
	}
	free_2d_array2((void ***)&split);
	return (ft_strdup(buffer));
}

static void	make_path2_1(char *buffer, int bf_idx)
{
	while (buffer[--bf_idx] != '/')
		;
	if (bf_idx == 0)
		buffer[++bf_idx] = '\0';
	else
		buffer[bf_idx] = '\0';
}

static char	*make_path2_2(char **split, int bf_idx, int idx, int mode)
{
	if (bf_idx + 2 + ft_strlen(split[idx]) > MAX_PATH_LEN)
	{
		if (mode == 0)
			err_msg_print3("cd: ", split[idx], ": File name too long");
		free_2d_array2((void ***)&split);
		return (T_NULL);
	}
	return ((char *)1);
}
