/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_and_or_bonus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:17:14 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:58:20 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static t_bool	case_pipe_and_or(char **cmd_ptr, int *idx);
static t_bool	wait_for_additional_cmd(char **cmd_ptr, char *cmd);
static void		wait_for_additional_cmd2(char *buffer);

t_bool	meet_and_or(char **cmd_ptr, int *idx)
{
	int		save_idx;
	char	*sliced_part;
	char	*cmd;

	cmd = *cmd_ptr;
	save_idx = (*idx);
	if ((cmd[(*idx)] == '&' && cmd[(*idx) + 1] == '&')
		|| (cmd[(*idx)] == '|' && cmd[(*idx) + 1] == '|'))
		(*idx)++;
	if (case_pipe_and_or(cmd_ptr, &(*idx)) == FALSE)
		return (FALSE);
	if (*cmd_ptr != cmd)
	{
		cmd = *cmd_ptr;
		(*idx) = save_idx;
		sliced_part = &cmd[(*idx)];
		if (check_syntax_error(&sliced_part, 1) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

static t_bool	case_pipe_and_or(char **cmd_ptr, int *idx)
{
	char	*cmd;

	cmd = *cmd_ptr;
	while (ft_isblank(cmd[++(*idx)]) == TRUE)
		;
	if (cmd[(*idx)] == '|' && cmd[(*idx) + 1] != '|')
		return (syntax_error_print("|"));
	else if (cmd[(*idx)] == '|' && cmd[(*idx) + 1] == '|')
		return (syntax_error_print("||"));
	else if (cmd[(*idx)] == '&' && cmd[(*idx) + 1] == '&')
		return (syntax_error_print("&&"));
	else if (cmd[(*idx)] == ')')
		return (syntax_error_print(")"));
	else if (cmd[(*idx)] == '\0')
		if (wait_for_additional_cmd(cmd_ptr, cmd) == FALSE)
			return (FALSE);
	return (TRUE);
}

static t_bool	wait_for_additional_cmd(char **cmd_ptr, char *cmd)
{
	char	*buffer[2];

	while (1)
	{
		buffer[0] = readline("> ");
		if (buffer[0] == T_NULL)
		{
			err_msg_print1("syntax error: unexpected end of file");
			return (FALSE);
		}
		else if (buffer[0][0] == '\0')
		{
			ft_free1((void **)&buffer[0]);
			continue ;
		}
		buffer[1] = ft_strjoin(cmd, buffer[0]);
		ft_free1((void **)&buffer[0]);
		ft_free1((void **)&cmd);
		*cmd_ptr = buffer[1];
		wait_for_additional_cmd2(buffer[1]);
		break ;
	}
	return (TRUE);
}

static void	wait_for_additional_cmd2(char *buffer)
{
	if (buffer == T_NULL)
	{
		err_msg_print1("malloc error");
		exit(1);
	}
}
