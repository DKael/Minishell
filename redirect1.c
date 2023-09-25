/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:25:30 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 16:43:37 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_rd_sign(char *str, int idx);
static int	case_heredoc(t_dll *heredoc_names, int tmp_fd);
extern int	case_in_redirect(char *file_name, int tmp_fd);
extern int	case_out_redirect(char *file_name, int tmp_fd, int mode);

int	sign_redirection(t_data *data, t_dll *tkns)
{
	int			redir_idx;
	int			tmp_fd;
	int			result;
	t_dllnode	*node_ptr;

	redir_idx = -1;
	while (++redir_idx < ((t_cmd_info *)(tkns->head.contents))->redir_cnt)
	{
		node_ptr = tkns->head.back;
		tmp_fd = check_fd((char *)node_ptr->contents);
		if (tmp_fd >= 256)
			return (1);
		result = do_redirect(data, tkns, tmp_fd, (char *)node_ptr->contents);
		if (result != 0)
			return (result);
		dll_del_node(tkns, node_ptr, str_delete_func);
	}
	return (0);
}

static int	get_rd_sign(char *str, int idx)
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

static int	case_heredoc(t_dll *heredoc_names, int tmp_fd)
{
	t_dllnode	*node_ptr;
	char		*tmp_str;
	int			tmp;
	t_file_info	f_info;

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

static int	check_fd(char *tmp_str)
{
	int	tmp_fd;

	tmp_fd = ft_atoi(tmp_str);
	if (tmp_fd >= 256)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		ft_putnbr_fd(tmp_fd, STDERR_FILENO);
		write(STDERR_FILENO, ": Bad file descriptor\n", 22);
	}
	return (tmp_fd);
}

static int	do_redirect(t_data *data, t_dll *tkns, int tmp_fd, char *tmp_str)
{
	int	idx;
	int	rd_sign;
	int	result;

	idx = -1;
	while (tmp_str[++idx] != '\0' && tmp_str[idx] != ' ')
		;
	rd_sign = get_rd_sign(tmp_str, idx);
	if (rd_sign == 0)
		result = case_heredoc(&(((t_cmd_info *)
						(tkns->head.contents))->heredoc_names), tmp_fd);
	else if (rd_sign == 1)
		result = case_in_redirect(&tmp_str[idx + 1], tmp_fd);
	else if (rd_sign == 2 || rd_sign == 3)
		result = case_out_redirect(&tmp_str[idx + 1], &tmp_fd, rd_sign);
	if (result != 0)
		return (result);
	data->opened_fd[tmp_fd] = tmp_fd;
	return (0);
}
