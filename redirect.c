/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:25:30 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/21 13:19:36 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool pipe_redirection(t_data *data, int ao_idx, int pp_idx)
{
	if (data->pp != T_NULL)
	{
		if (dup2(data->pp[pp_idx][0], STDIN_FILENO) != -1 && dup2(data->pp[pp_idx + 1][1], STDOUT_FILENO) != -1)
		{
			close_pipes(data, data->pipe_cnt[ao_idx]);
			return (TRUE);
		}
	}
	close_pipes(data, data->pipe_cnt[ao_idx]);
	return (FALSE);
}

static int get_rd_sign(char *str, int idx)
{
	if (str[idx - 1] == '<')
	{
		if (idx - 2 >= 0 && str[idx - 2] == '<')
			return (0);
		else
			return (1);
	}
	else
	{
		if (idx - 2 >= 0 && str[idx - 2] == '>')
			return (2);
		else
			return (3);
	}
}

int get_file_info(char *name, t_file_info *info, int mode)
{
	if (access(name, F_OK) == -1)
	{
		if (mode == 0)
			err_msg_print2(name, ": No such file or directory");
		return (1);
	}
	if (stat(name, &info->bf) == -1)
		return (4);
	info->mode = info->bf.st_mode;
	if (((info->bf.st_mode) & S_IFMT) == S_IFDIR)
		info->type = DIRECTORY;
	else if (((info->bf.st_mode) & S_IFREG) == S_IFDIR)
		info->type = REGULAR_FILE;
	else if (((info->bf.st_mode) & S_IFREG) == S_IFLNK)
		info->type = SYMBOLIC_LINK;
	else
		info->type = ETC;
	return (0);
}

int case_heredoc(t_dll *heredoc_names, int tmp_fd)
{
	t_dllnode *node_ptr;
	char *tmp_str;
	int tmp;
	t_file_info f_info;

	node_ptr = heredoc_names->head.back;
	tmp_str = (char *)(node_ptr->contents);
	tmp = get_file_info(tmp_str, &f_info, 0);
	if (tmp != 0)
		return (tmp);
	if ((f_info.mode & 0400) != 0400)
	{
		err_msg_print2(tmp_str, ": Permission denied");
		return (1);
	}
	tmp = open(tmp_str, O_RDONLY);
	if (tmp == -1)
		return (2);
	if (dup2(tmp, tmp_fd) == -1)
		return (3);
	dll_node_move_to_tail(node_ptr, heredoc_names);
	close(tmp);
	return (0);
}

int case_in_redirect(char *file_name, int tmp_fd)
{
	int tmp;
	t_file_info f_info;

	tmp = get_file_info(file_name, &f_info, 0);
	if (tmp != 0)
		return (tmp);
	if ((f_info.mode & 0400) != 0400)
	{
		err_msg_print2(file_name, ": Permission denied");
		return (1);
	}
	tmp = open(file_name, O_RDONLY);
	if (tmp == -1)
		return (2);
	if (dup2(tmp, tmp_fd) == -1)
		return (3);
	close(tmp);
	return (0);
}

int case_out_redirect(char *file_name, int tmp_fd, int mode)
{
	int tmp;
	t_file_info f_info;

	tmp = get_file_info(file_name, &f_info, 1);
	if (tmp == 4)
		return (tmp);
	if (tmp == 0)
	{
		if (f_info.type == DIRECTORY)
		{
			err_msg_print2(file_name, ": Is a directory");
			return (1);
		}
		if ((f_info.mode & 0200) != 0200)
		{
			err_msg_print2(file_name, ": Permission denied");
			return (1);
		}
	}
	if (tmp_fd == 0)
		tmp_fd = STDOUT_FILENO;
	if (mode == 2)
		tmp = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (mode == 3)
		tmp = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp == -1)
		return (2);
	if (dup2(tmp, tmp_fd) == -1)
		return (3);
	close(tmp);
	return (0);
}

int sign_redirection(t_data *data, t_dll *tkns)
{
	int redir_cnt;
	int redir_idx;
	char *tmp_str;
	int tmp_fd;
	int idx;
	int rd_sign;
	t_dllnode *node_ptr;
	int result;

	redir_cnt = ((t_cmd_info *)(tkns->head.contents))->redir_cnt;
	redir_idx = -1;
	while (++redir_idx < redir_cnt)
	{
		node_ptr = tkns->head.back;
		tmp_str = (char *)node_ptr->contents;
		tmp_fd = ft_atoi(tmp_str);
		if (tmp_fd >= 256)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			ft_putnbr_fd(tmp_fd, STDERR_FILENO);
			write(STDERR_FILENO, ": Bad file descriptor\n", 22);
			return (1);
		}
		idx = -1;
		while (tmp_str[++idx] != '\0' && tmp_str[idx] != ' ')
			;
		rd_sign = get_rd_sign(tmp_str, idx);
		if (rd_sign == 0)
			result = case_heredoc(&(((t_cmd_info *)(tkns->head.contents))->heredoc_names), tmp_fd);
		else if (rd_sign == 1)
			result = case_in_redirect(&tmp_str[idx + 1], tmp_fd);
		else if (rd_sign == 2 || rd_sign == 3)
			result = case_out_redirect(&tmp_str[idx + 1], tmp_fd, rd_sign);
		if (result != 0)
			return (result);
		if (!(0 <= tmp_fd && tmp_fd <= 2))
		{
			idx = -1;
			while (data->opened_fd[++idx] != -1)
				;
			data->opened_fd[idx] = tmp_fd;
		}
		dll_del_node(tkns, node_ptr, str_delete_func);
	}
	return (0);
}

t_bool basic_redirection_save(t_data *data)
{
	if (dup2(STDIN_FILENO, data->old_stdin) == -1 || dup2(STDOUT_FILENO, data->old_stdout) == -1 || dup2(STDERR_FILENO, data->old_stderr) == -1)
		return (FALSE);
	else
		return (TRUE);
}

t_bool basic_redirection_recover(t_data *data)
{
	if (dup2(data->old_stdin, STDIN_FILENO) == -1 || dup2(data->old_stdout, STDOUT_FILENO) == -1 || dup2(data->old_stderr, STDERR_FILENO) == -1)
		return (FALSE);
	close(data->old_stdin);
	close(data->old_stdout);
	close(data->old_stderr);
	return (TRUE);
}

void opened_fd_close(t_data *data)
{
	int idx;

	idx = -1;
	while (data->opened_fd[++idx] != -1)
		close(data->opened_fd[idx]);
}