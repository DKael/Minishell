/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 16:44:12 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/23 21:35:52 by junehyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	case_pipe_and_or(char **cmd_ptr, int *idx);
static t_bool	wait_for_additional_cmd(char **cmd_ptr, char *cmd);

int	pipe_and_ampersand(char *cmd, int idx, char **cmd_ptr)
{
	int		save_idx;
	char	*sliced_part;

	save_idx = idx;
	if ((cmd[idx] == '&' && cmd[idx + 1] == '&')
		|| (cmd[idx] == '|' && cmd[idx + 1] == '|'))
		idx++;
	if (case_pipe_and_or(cmd_ptr, &idx) == FALSE)
		return (FALSE);
	if (*cmd_ptr != cmd)
	{
		cmd = *cmd_ptr;
		idx = save_idx;
		sliced_part = &cmd[idx];
		if (check_syntax_error(&sliced_part, 1) == FALSE)
			return (FALSE);
	}
	return (idx);
}

int	redirect_and_quote(char *cmd, int idx)
{
	char	char_tmp;

	if (cmd[idx] == '<' || cmd[idx] == '>')
	{
		if ((cmd[idx] == '<' && cmd[idx + 1] == '<')
			|| (cmd[idx] == '>' && cmd[idx + 1] == '>'))
			idx++;
		if (case_lts_gts(cmd, &idx) == FALSE)
			return (FALSE);
	}
	else if (cmd[idx] == '\"' || cmd[idx] == '\'')
	{
		char_tmp = cmd[idx];
		while (cmd[++idx] != char_tmp)
			;
		idx++;
	}
	return (idx);
}

t_bool	check_special_char_syntax(char **cmd_ptr)
{
	char	*cmd;
	int		idx;

	cmd = *cmd_ptr;
	idx = 0;
	while (ft_isblank(cmd[idx]) == TRUE)
		idx++;
	if (pipe_and_or(cmd, idx) == FALSE)
		return (FALSE);
	while (cmd[idx] != '\0')
	{
		if (cmd[idx] == '|' || (cmd[idx] == '&' && cmd[idx + 1] == '&'))
		{
			idx = pipe_and_ampersand(cmd, idx, cmd_ptr);
			if (idx == FALSE)
				return (FALSE);
		}
		idx = redirect_and_quote(cmd, idx);
		if (idx == FALSE)
			return (FALSE);
		else
			idx++;
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
	char	*buffer;
	char	*temp;

	while (1)
	{
		buffer = readline("> ");
		if (buffer == T_NULL)
			unexpected_eod_print();
		else if (buffer[0] == '\0')
		{
			ft_free1((void **)&buffer);
			continue ;
		}
		temp = ft_strjoin(cmd, buffer);
		ft_free1((void **)buffer);
		ft_free1((void **)cmd);
		*cmd_ptr = temp;
		if (temp == T_NULL)
		{
			printf("minishell: malloc error!\n");
			exit(1);
		}
		break ;
	}
	return (TRUE);
}
