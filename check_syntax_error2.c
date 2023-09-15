/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:28:21 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/15 15:03:19 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool check_multiple_lines(const char *cmd)
{
	int idx;

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

t_bool check_quote_closed(const char *cmd)
{
	int idx;
	char tmp;

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

t_bool parentheses_in_parentheses(char *cmd, int *idx)
{
	int idx2;
	char char_tmp;
	t_bool empty_flag;
	int word_cnt;

	while (cmd[(*idx)] != ')')
	{
		idx2 = (*idx);
		word_cnt = 0;
		while (cmd[--idx2] != '(' && cmd[idx2] != '|' && cmd[idx2] != '&')
		{
			if (cmd[idx2] == '<' || cmd[idx2] == '>')
				return (syntax_error_print("("));
			else if (ft_isblank(cmd[idx2]) == FALSE)
			{
				if (ft_isblank(cmd[idx2]) == FALSE && ft_isblank(cmd[idx2 + 1]) == TRUE)
				{
					word_cnt++;
					if (word_cnt >= 2)
						return (syntax_error_print("("));
				}
				if (cmd[idx2] == '\"' || cmd[idx2] == '\'')
				{
					char_tmp = cmd[idx2];
					while (cmd[--idx2] != char_tmp)
						;
				}
			}
		}
		if (word_cnt == 1)
		{
			while (ft_isblank(cmd[++(*idx)]) == TRUE)
				;
			if (cmd[(*idx)] == ')')
			{
				printf("minishell: function not supported\n");
				return (FALSE);
			}
			else if (cmd[(*idx)] == '\0')
				return (syntax_error_print("newline"));
			idx2 = idx;
			while (cmd[(*idx)] != '\0' && cmd[(*idx)] != ')' && ft_isblank(cmd[(*idx)]) == FALSE)
			{
				if (cmd[(*idx)] == '\"' || cmd[(*idx)] == '\'')
					ignore_quote(cmd, &idx);
				(*idx)++;
			}
			cmd[(*idx)] = '\0';
			syntax_error_print(&cmd[idx2]);
			return (FALSE);
		}
		empty_flag = TRUE;
		while (cmd[++(*idx)] != '\0')
		{
			if (cmd[(*idx)] == ')')
				break;
			else if (ft_isblank(cmd[(*idx)]) == FALSE)
			{
				empty_flag = FALSE;
				if (cmd[(*idx)] == '\"' || cmd[(*idx)] == '\'')
					ignore_quote(cmd, idx);
				else if (cmd[(*idx)] == '(' && parentheses_in_parentheses(&cmd[(*idx)], idx) == FALSE)
					return (FALSE);
			}
		}
		(*idx)++;
	}
}

t_bool check_parentheses_syntax(char *cmd)
{
	int idx;
	int save_idx;
	char char_tmp;
	t_bool empty_flag;
	t_bool parentheses_flag;
	int word_cnt;

	parentheses_flag = FALSE;
	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '(')
		{
			if (parentheses_flag == FALSE)
				parentheses_flag = TRUE;
			else
				return (syntax_error_print("("));
			save_idx = idx;
			word_cnt = 0;
			while (--idx >= 0 && cmd[idx] != '|' && cmd[idx] != '&')
			{
				if (cmd[idx] == '<' || cmd[idx] == '>')
					return (syntax_error_print("("));
				else if (ft_isblank(cmd[idx]) == FALSE)
				{
					if (ft_isblank(cmd[idx]) == FALSE && ft_isblank(cmd[idx + 1]) == TRUE)
					{
						word_cnt++;
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
			if (word_cnt == 1)
			{
				idx = save_idx;
				while (ft_isblank(cmd[++idx]) == TRUE)
					;
				if (cmd[idx] == ')')
				{
					printf("minishell: function not supported\n");
					return (FALSE);
				}
				else if (cmd[idx] == '\0')
					return (syntax_error_print("newline"));
				save_idx = idx;
				while (cmd[idx] != '\0' && cmd[idx] != ')' && ft_isblank(cmd[idx]) == FALSE)
				{
					if (cmd[idx] == '\"' || cmd[idx] == '\'')
						ignore_quote(cmd, &idx);
					idx++;
				}
				cmd[idx] = '\0';
				syntax_error_print(&cmd[save_idx]);
				return (FALSE);
			}

			empty_flag = TRUE;
			idx = save_idx;
			while (cmd[++idx] != '\0')
			{
				if (cmd[idx] == ')')
					break;
				else if (ft_isblank(cmd[idx]) == FALSE)
				{
					empty_flag = FALSE;
					if (cmd[idx] == '\"' || cmd[idx] == '\'')
						ignore_quote(cmd, &idx);
					else if (cmd[idx] == '(' && parentheses_in_parentheses(&cmd[idx], &idx) == FALSE)
						return (FALSE);
				}
			}
			if (cmd[idx] == '\0')
			{
				printf("minishell: syntax error, unclosed parentheses\n");
				return (FALSE);
			}
			else if (empty_flag == TRUE)
				return (syntax_error_print(")"));
		}
		else if (parentheses_flag == FALSE && (cmd[idx] == '\"' || cmd[idx] == '\''))
			ignore_quote(cmd, &idx);
		else if (cmd[idx] == ')')
			return (syntax_error_print(")"));
		else if (parentheses_flag == TRUE && ft_isblank(cmd[idx]) == FALSE)
		{
			save_idx = idx;
			while (cmd[idx] != '\0' && cmd[idx] != '(' && cmd[idx] != ')' && ft_isblank(cmd[idx]) == FALSE && cmd[idx] != '|' && cmd[idx] != '&' && cmd[idx] != '<' && cmd[idx] != '>')
				idx++;
			if (cmd[idx] == '<' || cmd[idx] == '>')
			{
				if (ft_isndecimal(&cmd[save_idx], idx - save_idx) == FALSE)
				{
					cmd[idx] = '\0';
					return (syntax_error_print(&cmd[save_idx]));
				}
				if ((cmd[idx] == '<' && cmd[idx + 1] == '<') || (cmd[idx] == '>' && cmd[idx + 1] == '>'))
					idx++;
				if (case_lts_gts(cmd, &idx) == FALSE)
					return (FALSE);
				idx--;
			}
			else if ((cmd[idx] == '|' || cmd[idx] == '&') && save_idx == idx)
				parentheses_flag = FALSE;
			else
			{
				cmd[idx] = '\0';
				return (syntax_error_print(&cmd[save_idx]));
			}
		}
	}
	return (TRUE);
}

t_bool check_dollor_braces(char *cmd)
{
	int idx;
	int idx_chk;
	char char_tmp;
	t_bool bad_flag;

	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '$' && cmd[idx + 1] == '{')
		{
			idx_chk = idx;
			idx++;
			bad_flag = FALSE;
			while (cmd[++idx] != '}')
			{
				if (ft_isblank(cmd[idx]) == TRUE || cmd[idx] == '$')
					bad_flag = TRUE;
				else if (cmd[idx] == '\"' || cmd[idx] == '\'')
				{
					bad_flag = TRUE;
					char_tmp = cmd[idx];
					while (cmd[++idx] != char_tmp)
						;
				}
				else if (cmd[idx] == '\0')
				{
					write(2, "minishell: syntax error, unclosed brace\n", 41);
					return (FALSE);
				}
			}
			if (bad_flag || idx - idx_chk == 2)
			{
				write(2, "minishell: ", 12);
				write(2, &cmd[idx_chk], idx + 1);
				write(2, ": bad substitution\n", 20);
				return (FALSE);
			}
		}
	}
	return (TRUE);
}
