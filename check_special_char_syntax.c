/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_special_char_syntax.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 16:44:12 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/10 16:52:43 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool case_pipe_and_or(char **input_ptr, int *idx);
static t_bool wait_for_additional_input(char **input_ptr, char *input);
static t_bool case_lts_gts(char *input, int *idx);

t_bool check_special_char_syntax(char **input_ptr)
{
	char *input;
	char *sliced_part;
	int idx;
	int save_idx;
	char char_tmp;

	input = *input_ptr;
	idx = 0;
	while (ft_isblank(input[idx]) == TRUE)
		idx++;
	if (input[idx] == '|')
	{
		if (input[idx + 1] == '|')
			return (syntax_error_print("||"));
		else
			return (syntax_error_print("|"));
	}
	else if (input[idx] == '&' && input[idx + 1] == '&')
		return (syntax_error_print("&&"));

	while (input[idx] != '\0')
	{
		if (input[idx] == '|' || (input[idx] == '&' && input[idx + 1] == '&'))
		{
			save_idx = idx;
			if ((input[idx] == '&' && input[idx + 1] == '&') || (input[idx] == '|' && input[idx + 1] == '|'))
				idx++;
			if (case_pipe_and_or(input_ptr, &idx) == FALSE)
				return (FALSE);
			if (*input_ptr != input)
			{
				input = *input_ptr;
				idx = save_idx;
				if (check_multiple_lines(&input[idx]) == FALSE 
					|| check_quote_closed(&input[idx]) == FALSE
					|| check_parentheses_syntax(&input[idx]) == FALSE)
					return (FALSE);
			}
		}
		else if (input[idx] == '<' || input[idx] == '>')
		{
			if ((input[idx] == '<' && input[idx + 1] == '<') || (input[idx] == '>' && input[idx + 1] == '>'))
				idx++;
			if (case_lts_gts(input, &idx) == FALSE)
				return (FALSE);
		}
		else if (input[idx] == '\"' || input[idx] == '\'')
		{
			char_tmp = input[idx];
			while (input[++idx] != char_tmp)
				;
			idx++;
		}
		else if (input[idx] == '(')
		{
			save_idx = idx;
			while (input[++idx] != ')')
			{
				if (input[idx] == '\"' || input[idx] == '\'')
				{
					char_tmp = input[idx];
					while (input[++idx] != char_tmp)
						;
				}
			}
			sliced_part = ft_strndup(&input[save_idx + 1], idx - save_idx - 1);
			if (sliced_part == T_NULL)
			{
				printf("minishell: malloc error!\n");
				free(input);
				exit(1);
			}
			if (check_special_char_syntax(&sliced_part) == FALSE)
			{
				free(sliced_part);
				return (FALSE);
			}
			free(sliced_part);
			idx++;
		}
		else
			idx++;
	}
	return (TRUE);
}

static t_bool case_pipe_and_or(char **input_ptr, int *idx)
{
	char *input;

	input = *input_ptr;
	while (ft_isblank(input[++(*idx)]) == TRUE)
		;
	if (input[(*idx)] == '|' && input[(*idx) + 1] != '|')
		return (syntax_error_print("|"));
	else if (input[(*idx)] == '|' && input[(*idx) + 1] == '|')
		return (syntax_error_print("||"));
	else if (input[(*idx)] == '&' && input[(*idx) + 1] == '&')
		return (syntax_error_print("&&"));
	else if (input[(*idx)] == '\0')
		if (wait_for_additional_input(input_ptr, input) == FALSE)
			return (FALSE);
	return (TRUE);
}

static t_bool wait_for_additional_input(char **input_ptr, char *input)
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
		temp = ft_strjoin(input, buffer);
		free(buffer);
		free(input);
		*input_ptr = temp;
		if (temp == T_NULL)
		{
			printf("minishell: malloc error!\n");
			exit(1);
		}
		break;
	}
	return (TRUE);
}

static t_bool case_lts_gts(char *input, int *idx)
{
	int check;
	char *temp;

	while (ft_isblank(input[++(*idx)]) == TRUE)
		;
	if (input[(*idx)] == '>' && input[(*idx) + 1] != '>')
		return (syntax_error_print(">"));
	else if (input[(*idx)] == '>' && input[(*idx) + 1] == '>')
		return (syntax_error_print(">>"));
	else if (input[(*idx)] == '<' && input[(*idx) + 1] != '<')
		return (syntax_error_print("<"));
	else if (input[(*idx)] == '<' && input[(*idx) + 1] == '<')
		return (syntax_error_print("<<"));
	else if (input[(*idx)] == '|' && input[(*idx) + 1] != '|')
		return (syntax_error_print("|"));
	else if (input[(*idx)] == '|' && input[(*idx) + 1] == '|')
		return (syntax_error_print("||"));
	else if (input[(*idx)] == '&' && input[(*idx) + 1] == '&')
		return (syntax_error_print("&&"));
	else if (input[(*idx)] == '\0')
		return (syntax_error_print("newline"));
	check = (*idx);
	while (input[++(*idx)] != '<' && input[(*idx)] != '>'
		&& input[(*idx)] != '\0' && input[(*idx)] != '|'
		&& !(input[(*idx)] == '&' && input[(*idx) + 1] == '&')
		&& ft_isblank(input[(*idx)]) == FALSE)
		;
	if (input[(*idx)] == '<' || input[(*idx)] == '>')
	{
		temp = ft_strndup(&input[check], (*idx) - check);
		if (temp == T_NULL)
		{
			printf("minishell: malloc error!\n");
			free(input);
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
