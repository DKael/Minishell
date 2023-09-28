/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_parentheses4.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 02:29:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 02:40:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	t_bool	before_meet_close_parentheses2_1(char *cmd, int *idx);
static	t_bool	before_meet_close_parentheses2_2(char *cmd, int *idx, int idx2);

t_bool	before_meet_close_parentheses(char *cmd, int *idx)
{
	int	idx2;

	while (cmd[++(*idx)] != ')' && cmd[(*idx)] != '|'
		&& cmd[(*idx)] != '&' && cmd[(*idx)] != '\0')
	{
		if (ft_isblank(cmd[(*idx)]) == TRUE)
			continue ;
		idx2 = (*idx);
		if (before_meet_close_parentheses2_1(cmd, idx) == FALSE)
			return (FALSE);
		if (before_meet_close_parentheses2_2(cmd, idx, idx2) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

static	t_bool	before_meet_close_parentheses2_1(char *cmd, int *idx)
{
	if (cmd[(*idx)] == '(')
		return (syntax_error_print("("));
	while (cmd[(*idx)] != '(' && cmd[(*idx)] != ')'
		&& ft_isblank(cmd[(*idx)]) == FALSE && cmd[(*idx)] != '<'
		&& cmd[(*idx)] != '>' && cmd[(*idx)] != '\0')
	{
		if (cmd[(*idx)] == '\'' || cmd[(*idx)] == '\"')
			ignore_quote(cmd, idx);
		(*idx)++;
	}
	return (TRUE);
}

static	t_bool	before_meet_close_parentheses2_2(char *cmd, int *idx, int idx2)
{
	if (cmd[(*idx)] == '<' || cmd[(*idx)] == '>')
	{
		if (ft_isndecimal(&cmd[idx2], (*idx) - idx2) == FALSE)
		{
			cmd[(*idx)] = '\0';
			return (syntax_error_print(&cmd[idx2]));
		}
		if ((cmd[(*idx)] == '<' && cmd[(*idx) + 1] == '<')
			|| (cmd[(*idx)] == '>' && cmd[(*idx) + 1] == '>'))
			(*idx)++;
		if (case_lts_gts(cmd, idx) == FALSE)
			return (FALSE);
		(*idx)--;
	}
	else
	{
		cmd[(*idx)] = '\0';
		return (syntax_error_print(&cmd[idx2]));
	}
	return (TRUE);
}
