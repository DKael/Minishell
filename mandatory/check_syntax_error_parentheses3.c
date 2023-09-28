/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_parentheses3.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 01:33:00 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:37:17 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	check_front(char *cmd, int idx, int *word_cnt);
static t_bool	word_cnt_one(char *cmd, int idx);
static t_bool	check_back(char *cmd, int *idx);
extern t_bool	before_meet_close_parentheses(char *cmd, int *idx);

t_bool	parentheses_in_parentheses(char *cmd, int *idx)
{
	int	word_cnt;

	word_cnt = 0;
	if (check_front(cmd, *idx, &word_cnt) == FALSE)
		return (FALSE);
	if (word_cnt == 1)
		return (word_cnt_one(cmd, *idx));
	if (check_back(cmd, idx) == FALSE
		|| before_meet_close_parentheses(cmd, idx) == FALSE)
		return (FALSE);
	if (cmd[(*idx)] == '\0')
	{
		err_msg_print1("syntax error, unclosed parentheses");
		return (FALSE);
	}
	else if (cmd[(*idx)] == ')')
		(*idx)--;
	return (TRUE);
}

static t_bool	check_front(char *cmd, int idx, int *word_cnt)
{
	char	char_tmp;

	while (cmd[--idx] != '(' && cmd[idx] != '|' && cmd[idx] != '&')
	{
		if (cmd[idx] == '<' || cmd[idx] == '>')
			return (syntax_error_print("("));
		else if (ft_isblank(cmd[idx]) == FALSE)
		{
			if (ft_isblank(cmd[idx]) == FALSE
				&& (ft_isblank(cmd[idx + 1]) == '('
					|| ft_isblank(cmd[idx + 1]) == TRUE))
				(*word_cnt)++;
			if ((*word_cnt) >= 2)
				return (syntax_error_print("("));
		}
		if (cmd[idx] == '\"' || cmd[idx] == '\'')
		{
			char_tmp = cmd[idx];
			while (cmd[--idx] != char_tmp)
				;
		}
	}
	return (TRUE);
}

static t_bool	word_cnt_one(char *cmd, int idx)
{
	int	idx2;

	while (ft_isblank(cmd[++idx]) == TRUE)
		;
	if (cmd[idx] == ')')
	{
		err_msg_print1("function not supported");
		return (FALSE);
	}
	else if (cmd[idx] == '\0')
	{
		err_msg_print1("syntax error, unclosed parentheses");
		return (FALSE);
	}
	idx2 = idx;
	while (cmd[idx] != '\0' && cmd[idx] != ')'
		&& cmd[idx] != '(' && ft_isblank(cmd[idx]) == FALSE)
	{
		if (cmd[idx] == '\"' || cmd[idx] == '\'')
			ignore_quote(cmd, &idx);
		idx++;
	}
	cmd[idx] = '\0';
	return (syntax_error_print(&cmd[idx2]));
}

static t_bool	check_back(char *cmd, int *idx)
{
	t_bool	empty_flag;

	empty_flag = TRUE;
	while (cmd[++(*idx)] != ')' && cmd[(*idx)] != '\0')
	{
		if (ft_isblank(cmd[(*idx)]) == FALSE)
		{
			empty_flag = FALSE;
			if (cmd[(*idx)] == '\"' || cmd[(*idx)] == '\'')
				ignore_quote(cmd, idx);
			else if (cmd[(*idx)] == '('
				&& parentheses_in_parentheses(cmd, idx) == FALSE)
				return (FALSE);
		}
	}
	if (cmd[(*idx)] == '\0')
	{
		err_msg_print1("syntax error, unclosed parentheses");
		return (FALSE);
	}
	else if (empty_flag == TRUE)
		return (syntax_error_print(")"));
	return (TRUE);
}
