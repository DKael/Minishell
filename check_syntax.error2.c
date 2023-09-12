/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.error2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:28:21 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/12 19:43:38 by hyungdki         ###   ########.fr       */
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

t_bool check_parentheses_syntax(char *cmd)
{
	int idx;
	int save_idx;
	char char_tmp;
	t_bool empty_flag;
	int word_cnt;
	char *sliced_part;

	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '(')
		{
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
					{
						char_tmp = cmd[idx];
						while (cmd[++idx] != char_tmp)
							;
					}
					idx++;
				}

				sliced_part = ft_strndup(&cmd[save_idx], idx - save_idx);
				if (sliced_part == T_NULL)
				{
					printf("minishell: malloc error!\n");
					free(cmd);
					exit(1);
				}
				syntax_error_print(sliced_part);
				free(sliced_part);
				return (FALSE);
			}

			empty_flag = TRUE;
			idx = save_idx;
			while (cmd[++idx] != '\0')
			{
				if (cmd[idx] == ')')
					break;
				else if (cmd[idx] == '\"' || cmd[idx] == '\'')
				{
					empty_flag = FALSE;
					char_tmp = cmd[idx];
					while (cmd[++idx] != char_tmp)
						;
				}
				else if (ft_isblank(cmd[idx]) == FALSE)
					empty_flag = FALSE;
			}
			if (cmd[idx] == '\0')
			{
				printf("minishell: syntax error, unclosed parentheses\n");
				return (FALSE);
			}
			else if (empty_flag == TRUE)
				return (syntax_error_print(")"));
		}
		else if (cmd[idx] == '\"' || cmd[idx] == '\'')
		{
			char_tmp = cmd[idx];
			while (cmd[++idx] != char_tmp)
				;
		}
		else if (cmd[idx] == ')')
			return (syntax_error_print(")"));
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