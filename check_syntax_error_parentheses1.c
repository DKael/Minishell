/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_parentheses1.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:28:21 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 02:36:57 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	meet_open_parentheses(char *cmd, int *idx,
					t_bool *parentheses_flag);
static t_bool	check_front_of_parentheses(char *cmd, int idx, int *word_cnt);
static t_bool	word_cnt_is_one(char *cmd, int save_idx);
static t_bool	check_back_of_parentheses(char *cmd, int *idx, int save_idx);
extern t_bool	meet_something_behind_of_parentheses(char *cmd,
					int *idx, t_bool *parentheses_flag);
extern t_bool	parentheses_in_parentheses(char *cmd, int *idx);

t_bool	check_parentheses_syntax(char *cmd)
{
	int		idx;
	t_bool	parentheses_flag;

	parentheses_flag = FALSE;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if ((cmd[idx] == '(')
			&& meet_open_parentheses(cmd, &idx, &parentheses_flag) == FALSE)
			return (FALSE);
		else if (parentheses_flag == FALSE && (cmd[idx] == '\"'
				|| cmd[idx] == '\''))
			ignore_quote(cmd, &idx);
		else if (cmd[idx] == ')')
			return (syntax_error_print(")"));
		else if ((parentheses_flag == TRUE && ft_isblank(cmd[idx]) == FALSE)
			&& meet_something_behind_of_parentheses(cmd, &idx,
				&parentheses_flag) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

static t_bool	meet_open_parentheses(char *cmd, int *idx,
					t_bool *parentheses_flag)
{
	int	save_idx;
	int	word_cnt;

	if ((*parentheses_flag) == FALSE)
		(*parentheses_flag) = TRUE;
	else
		return (syntax_error_print("("));
	save_idx = (*idx);
	word_cnt = 0;
	if (check_front_of_parentheses(cmd, *idx, &word_cnt) == FALSE)
		return (FALSE);
	if (word_cnt == 1)
		return (word_cnt_is_one(cmd, save_idx));
	if (check_back_of_parentheses(cmd, idx, save_idx) == FALSE)
		return (FALSE);
	return (TRUE);
}

static t_bool	check_front_of_parentheses(char *cmd, int idx, int *word_cnt)
{
	char	char_tmp;

	while (--idx >= 0 && cmd[idx] != '|' && cmd[idx] != '&')
	{
		if (cmd[idx] == '<' || cmd[idx] == '>')
			return (syntax_error_print("("));
		else if (ft_isblank(cmd[idx]) == FALSE)
		{
			if (ft_isblank(cmd[idx]) == FALSE
				&& (ft_isblank(cmd[idx + 1]) == '('
					|| ft_isblank(cmd[idx + 1]) == TRUE))
			{
				(*word_cnt)++;
				if (word_cnt >= 2)
					return (syntax_error_print("("));
			}
			if (cmd[idx] == '\"' || cmd[idx] == '\'')
			{
				char_tmp = cmd[idx];
				while (cmd[--idx] != char_tmp)
					;
			}
		}
	}
	return (TRUE);
}

static t_bool	word_cnt_is_one(char *cmd, int save_idx)
{
	int	idx;

	idx = save_idx;
	while (ft_isblank(cmd[++idx]) == TRUE)
		;
	if (cmd[idx] == ')')
	{
		err_msg_print1("function not supported");
		return (FALSE);
	}
	else if (cmd[idx] == '\0')
		return (syntax_error_print("newline"));
	save_idx = idx;
	while (cmd[idx] != '\0' && cmd[idx] != ')'
		&& ft_isblank(cmd[idx]) == FALSE)
	{
		if (cmd[idx] == '\"' || cmd[idx] == '\'')
			ignore_quote(cmd, &idx);
		idx++;
	}
	cmd[idx] = '\0';
	return (syntax_error_print(&cmd[save_idx]));
}

static t_bool	check_back_of_parentheses(char *cmd, int *idx, int save_idx)
{
	t_bool	empty_flag;

	empty_flag = TRUE;
	(*idx) = save_idx;
	while (cmd[++(*idx)] != '\0' && cmd[(*idx)] != ')')
	{
		if (ft_isblank(cmd[(*idx)]) == FALSE)
		{
			empty_flag = FALSE;
			if (cmd[(*idx)] == '\"' || cmd[(*idx)] == '\'')
				ignore_quote(cmd, &(*idx));
			else if (cmd[(*idx)] == '('
				&& parentheses_in_parentheses(cmd, &(*idx)) == FALSE)
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
