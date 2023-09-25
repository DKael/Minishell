/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:59:00 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 15:58:59 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	case_in_redirect2(char *file_name, char *abs_file_name,
				int tmp_fd);
static int	case_out_redirect2(char *file_name, char *abs_file_name,
				int tmp_fd, int mode);
static int	case_out_redirect3(char *abs_file_name, int tmp_fd,
				int mode);

int	case_in_redirect(char *file_name, int tmp_fd)
{
	char	*abs_file_name;

	if (file_name[0] != '/')
		abs_file_name = make_path(file_name, 1);
	else
		abs_file_name = ft_strdup(file_name);
	if (abs_file_name == (char *)-1)
	{
		err_msg_print2(file_name, ": No such file or directory");
		return (1);
	}
	else if (abs_file_name == (char *)-2)
	{
		err_msg_print2(file_name, ": File name too long");
		return (1);
	}
	else if (abs_file_name == T_NULL)
		return (-1);
	return (case_in_redirect2(file_name, abs_file_name, tmp_fd));
}

static int	case_in_redirect2(char *file_name, char *abs_file_name, int tmp_fd)
{
	t_file_info	f_info;
	int			tmp;

	tmp = get_file_info(abs_file_name, &f_info, 0);
	if (tmp != 0)
		return (ft_free3((void **)&abs_file_name, tmp));
	if ((f_info.mode & 0400) != 0400)
	{
		err_msg_print2(file_name, ": Permission denied");
		return (ft_free3((void **)&abs_file_name, 1));
	}
	tmp = open(abs_file_name, O_RDONLY);
	if (tmp == -1)
		return (ft_free3((void **)&abs_file_name, 2));
	if (dup2(tmp, tmp_fd) == -1)
		return (ft_free3((void **)&abs_file_name, 3));
	close(tmp);
	return (ft_free3((void **)&abs_file_name, 0));
}

int	case_out_redirect(char *file_name, int *tmp_fd, int mode)
{
	char	*abs_file_name;

	if ((*tmp_fd) == 0)
		(*tmp_fd) = STDOUT_FILENO;
	if (file_name[0] != '/')
		abs_file_name = make_path(file_name, 1);
	else
		abs_file_name = ft_strdup(file_name);
	if (abs_file_name == (char *)-1)
	{
		err_msg_print2(file_name, ": No such file or directory");
		return (1);
	}
	else if (abs_file_name == (char *)-2)
	{
		err_msg_print2(file_name, ": File name too long");
		return (1);
	}
	else if (abs_file_name == T_NULL)
		return (-1);
	return (case_out_redirect2(file_name, abs_file_name, *tmp_fd, mode));
}

static int	case_out_redirect2(char *file_name, char *abs_file_name,
	int tmp_fd, int mode)
{
	int			tmp;
	t_file_info	f_info;

	tmp = get_file_info(abs_file_name, &f_info, 1);
	if (tmp == 4)
		return (ft_free3((void **)&abs_file_name, tmp));
	if (tmp == 0)
	{
		if (f_info.type == DIRECTORY)
		{
			err_msg_print2(file_name, ": Is a directory");
			return (ft_free3((void **)&abs_file_name, 1));
		}
		if ((f_info.mode & 0200) != 0200)
		{
			err_msg_print2(file_name, ": Permission denied");
			return (ft_free3((void **)&abs_file_name, 1));
		}
	}
	return (case_out_redirect3(abs_file_name, tmp_fd, mode));
}

static int	case_out_redirect3(char *abs_file_name, int tmp_fd, int mode)
{
	int	tmp;

	if (mode == 2)
		tmp = open(abs_file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (mode == 3)
		tmp = open(abs_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp == -1)
		return (ft_free3((void **)&abs_file_name, 2));
	if (dup2(tmp, tmp_fd) == -1)
		return (ft_free3((void **)&abs_file_name, 3));
	close(tmp);
	return (ft_free3((void **)&abs_file_name, 0));
}
