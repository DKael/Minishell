/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child1_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 16:28:59 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:58:32 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	child_init(t_data *data, int ao_idx, int pp_idx, t_cdata *cdata);
static void	child_redirection(t_cdata *cdata, t_data *data,
				int ao_idx, int pp_idx);
static void	child_execute_cmd1(t_data *data, t_cdata *cdata);
static void	child_execute_cmd2(t_data *data, t_cdata *cdata);
extern void	child_execute_builtin(t_data *data, t_cdata *cdata, int func_type);
extern char	*get_cmd_path(char **path, char *cmd);
extern int	child_parentheses(t_data *data, t_dll *heredoc, char *cmd);

void	child(t_data *data, int ao_idx, int pp_idx)
{
	t_cdata		cdata;
	t_dllnode	*node_ptr;
	int			idx;

	child_init(data, ao_idx, pp_idx, &cdata);
	child_redirection(&cdata, data, ao_idx, pp_idx);
	node_ptr = cdata.tkns->head.back;
	cdata.cmd = (char *)(node_ptr->contents);
	if (cdata.cmd_info->parentheses_flag == TRUE)
		child_parentheses(data, &cdata.cmd_info->heredoc_names, cdata.cmd);
	else
	{
		idx = is_builtin_func(cdata.cmd);
		if (idx == 0)
			child_execute_cmd1(data, &cdata);
		else
			child_execute_builtin(data, &cdata, idx);
	}
}

static void	child_init(t_data *data, int ao_idx, int pp_idx, t_cdata *cdata)
{
	make_command_str();
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cdata->tkns = data->tkn[ao_idx][pp_idx];
	cdata->cmd_info = (t_cmd_info *)(cdata->tkns->head.contents);
	cdata->pipe_cnt = data->pipe_cnt[ao_idx];
	cdata->raw_path = T_NULL;
	cdata->split_path = T_NULL;
	cdata->cmd_path = T_NULL;
	cdata->cmd = T_NULL;
	cdata->argu_lst = T_NULL;
	cdata->envp_lst = T_NULL;
}

static void	child_redirection(t_cdata *cdata, t_data *data,
	int ao_idx, int pp_idx)
{
	int	result;

	if (data->pipe_cnt[ao_idx] > 1
		&& pipe_redirection(data, ao_idx, pp_idx) == FALSE)
		child_exit(data, cdata, 1, "redirection error");
	result = sign_redirection(data, cdata->tkns);
	if (result == 1)
		exit(1);
	else if (result == 2)
		child_exit(data, cdata, 1, "file open error");
	else if (result == 3)
		child_exit(data, cdata, 1, "redirection error");
	else if (result == 4)
		child_exit(data, cdata, 1, "stat() error");
}

static void	child_execute_cmd1(t_data *data, t_cdata *cdata)
{
	if (cdata->cmd[0] == '\0')
	{
		err_msg_print1(": command not found");
		child_exit(data, cdata, 127, T_NULL);
	}
	cdata->raw_path = ft_getenv(data, "PATH");
	if (cdata->raw_path != T_NULL)
	{
		cdata->split_path = ft_split(cdata->raw_path, ':');
		if (cdata->split_path == T_NULL)
			child_exit(data, cdata, 1, "malloc error");
	}
	else
		cdata->split_path = T_NULL;
	cdata->cmd_path = get_cmd_path(cdata->split_path, cdata->cmd);
	if (cdata->cmd_path == (char *)-1)
		child_exit(data, cdata, 1, "malloc error");
	child_execute_cmd2(data, cdata);
}

static void	child_execute_cmd2(t_data *data, t_cdata *cdata)
{
	if (cdata->cmd_path == (char *)-1)
		child_exit(data, cdata, 1, "malloc error");
	if (cdata->cmd_path == T_NULL && cdata->split_path != T_NULL)
	{
		err_msg_print2(cdata->cmd, ": command not found");
		child_exit(data, cdata, 127, T_NULL);
	}
	else if (cdata->cmd_path == T_NULL && cdata->split_path == T_NULL)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		perror(cdata->cmd);
		child_exit(data, cdata, 127, T_NULL);
	}
	cdata->argu_lst = make_2d_array_from_dll(cdata->tkns);
	cdata->envp_lst = make_2d_envp_from_dll(&data->envdll);
	if (cdata->argu_lst == T_NULL || cdata->envp_lst == T_NULL)
		child_exit(data, cdata, 1, "malloc error");
	if (execve(cdata->cmd_path, cdata->argu_lst, cdata->envp_lst) == -1)
	{
		err_msg_print2(cdata->cmd_path, ": command not found");
		child_exit(data, cdata, 1, T_NULL);
	}
}
