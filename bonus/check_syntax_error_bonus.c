/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 16:44:12 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:58:21 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static t_bool	check_multiple_lines(const char *cmd);
static t_bool	check_quote_closed(const char *cmd);
extern t_bool	check_parentheses_syntax(char *cmd);
extern t_bool	check_dollor_braces(char *cmd);
static t_bool	check_special_char_syntax(char **cmd_ptr);
static t_bool	check_first_appear(char *cmd, int *idx);
extern t_bool	meet_lts_gts(char *cmd, int *idx);
extern t_bool	meet_and_or(char **cmd_ptr, int *idx);

t_bool	check_syntax_error(char **cmd, int mode)
{
	if (check_multiple_lines(*cmd) == FALSE
		|| check_quote_closed(*cmd) == FALSE
		|| check_parentheses_syntax(*cmd) == FALSE
		|| check_dollor_braces(*cmd) == FALSE)
		return (FALSE);
	if (mode == 0)
		if ((check_special_char_syntax(cmd) == FALSE))
			return (FALSE);
	return (TRUE);
}

static t_bool	check_multiple_lines(const char *cmd)
{
	int	idx;

	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '\n')
		{
			printf("minishell: syntax error, multiple lines input\n");
			return (FALSE);
		}
	}
	return (TRUE);
}

static t_bool	check_quote_closed(const char *cmd)
{
	int		idx;
	char	tmp;

	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '\"' || cmd[idx] == '\'')
		{
			tmp = cmd[idx];
			while (cmd[++idx] != tmp && cmd[idx] != '\0')
				;
			if (cmd[idx] == '\0')
			{
				printf("minishell: syntax error, unclosed quote\n");
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

static t_bool	check_special_char_syntax(char **cmd_ptr)
{
	int		idx;
	char	*cmd;

	cmd = *cmd_ptr;
	if (check_first_appear(cmd, &idx) == FALSE)
		return (FALSE);
	while (cmd[idx] != '\0')
	{
		if ((cmd[idx] == '|' || (cmd[idx] == '&' && cmd[idx + 1] == '&'))
			&& meet_and_or(cmd_ptr, &idx) == FALSE)
			return (FALSE);
		else if ((cmd[idx] == '<' || cmd[idx] == '>')
			&& meet_lts_gts(cmd, &idx) == FALSE)
			return (FALSE);
		else if (cmd[idx] == '\"' || cmd[idx] == '\'')
		{
			ignore_quote(cmd, &idx);
			idx++;
		}
		else
			idx++;
	}
	return (TRUE);
}

static t_bool	check_first_appear(char *cmd, int *idx)
{
	(*idx) = 0;
	while (ft_isblank(cmd[(*idx)]) == TRUE)
		(*idx)++;
	if (cmd[(*idx)] == '|')
	{
		if (cmd[(*idx) + 1] == '|')
			return (syntax_error_print("||"));
		else
			return (syntax_error_print("|"));
	}
	else if (cmd[(*idx)] == '&' && cmd[(*idx) + 1] == '&')
		return (syntax_error_print("&&"));
	return (TRUE);
}
