/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 16:44:12 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/15 13:40:58 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool case_pipe_and_or(char **cmd_ptr, int *idx);
static t_bool wait_for_additional_cmd(char **cmd_ptr, char *cmd);


t_bool check_special_char_syntax(char **cmd_ptr)
{
	char *cmd;
	char *sliced_part;
	int idx;
	int save_idx;
	char char_tmp;

	cmd = *cmd_ptr;
	idx = 0;
	while (ft_isblank(cmd[idx]) == TRUE)
		idx++;
	if (cmd[idx] == '|')
	{
		if (cmd[idx + 1] == '|')
			return (syntax_error_print("||"));
		else
			return (syntax_error_print("|"));
	}
	else if (cmd[idx] == '&' && cmd[idx + 1] == '&')
		return (syntax_error_print("&&"));

	while (cmd[idx] != '\0')
	{
		if (cmd[idx] == '|' || (cmd[idx] == '&' && cmd[idx + 1] == '&'))
		{
			save_idx = idx;
			if ((cmd[idx] == '&' && cmd[idx + 1] == '&') || (cmd[idx] == '|' && cmd[idx + 1] == '|'))
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
		}
		else if (cmd[idx] == '<' || cmd[idx] == '>')
		{
			if ((cmd[idx] == '<' && cmd[idx + 1] == '<') || (cmd[idx] == '>' && cmd[idx + 1] == '>'))
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
		else
			idx++;
	}
	return (TRUE);
}

static t_bool case_pipe_and_or(char **cmd_ptr, int *idx)
{
	char *cmd;

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

static t_bool wait_for_additional_cmd(char **cmd_ptr, char *cmd)
{
	char *buffer;
	char *temp;

	while (1)
	{
		buffer = readline("> ");
		if (buffer == T_NULL)
		{
			printf("minishell: syntax error: unexpected end of file\n");
			return (FALSE);
		}
		else if (buffer[0] == '\0')
		{
			free(buffer);
			continue;
		}
		temp = ft_strjoin(cmd, buffer);
		free(buffer);
		free(cmd);
		*cmd_ptr = temp;
		if (temp == T_NULL)
		{
			printf("minishell: malloc error!\n");
			exit(1);
		}
		break;
	}
	return (TRUE);
}

t_bool case_lts_gts(char *cmd, int *idx)
{
	int check;
	char *temp;

	while (ft_isblank(cmd[++(*idx)]) == TRUE)
		;
	if (cmd[(*idx)] == '>' && cmd[(*idx) + 1] != '>')
		return (syntax_error_print(">"));
	else if (cmd[(*idx)] == '>' && cmd[(*idx) + 1] == '>')
		return (syntax_error_print(">>"));
	else if (cmd[(*idx)] == '<' && cmd[(*idx) + 1] != '<')
		return (syntax_error_print("<"));
	else if (cmd[(*idx)] == '<' && cmd[(*idx) + 1] == '<')
		return (syntax_error_print("<<"));
	else if (cmd[(*idx)] == '|' && cmd[(*idx) + 1] != '|')
		return (syntax_error_print("|"));
	else if (cmd[(*idx)] == '|' && cmd[(*idx) + 1] == '|')
		return (syntax_error_print("||"));
	else if (cmd[(*idx)] == '&' && cmd[(*idx) + 1] == '&')
		return (syntax_error_print("&&"));
	else if (cmd[(*idx)] == ')')
		return (syntax_error_print(")"));
	else if (cmd[(*idx)] == '\0')
		return (syntax_error_print("newline"));
	check = (*idx);
	while (cmd[++(*idx)] != '<' && cmd[(*idx)] != '>'
		&& cmd[(*idx)] != '\0' && cmd[(*idx)] != '|'
		&& !(cmd[(*idx)] == '&' && cmd[(*idx) + 1] == '&')
		&& ft_isblank(cmd[(*idx)]) == FALSE)
		;
	if (cmd[(*idx)] == '<' || cmd[(*idx)] == '>')
	{
		temp = ft_strndup(&cmd[check], (*idx) - check);
		if (temp == T_NULL)
		{
			printf("minishell: malloc error!\n");
			free(cmd);
			exit(1);
		}
		if (ft_isdecimal(temp) == TRUE)
		{
			syntax_error_print(temp);
			free(temp);
			return (FALSE);
		}
		free(temp);
	}
	return (TRUE);
}
