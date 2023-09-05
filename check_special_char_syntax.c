/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_special_char_syntax.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 16:44:12 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/04 18:11:29 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool syntax_error_print(char *chr);
static t_bool case_pipe_and_or(char **input_ptr, int *idx);
static t_bool wait_for_additional_input(char **input_ptr, char *input);
static t_bool case_lts_gts(char *input, int *idx);

t_bool check_special_char_syntax(char *input)
{
	int idx;

	idx = 0;
	while ((9 <= input[idx] && input[idx] <= 13) || input[idx] == ' ')
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
			if ((input[idx] == '&' && input[idx + 1] == '&')
			|| (input[idx] == '|' && input[idx + 1] == '|'))
				idx++;
			if (case_pipe_and_or(&input, &idx) == FALSE)
				return (FALSE);
		}
		else if (input[idx] == '<' || input[idx] == '>')
		{
			if ((input[idx] == '<' && input[idx + 1] == '<')
				|| (input[idx] == '>' && input[idx + 1] == '>'))
				idx++;
			if (case_lts_gts(input, &idx) == FALSE)
				return (FALSE);
		}
		else
			idx++;
	}
	return (TRUE);
}

static t_bool syntax_error_print(char *chr)
{
	printf("minishell: syntax error near unexpected token `%s'\n", chr);
	return (FALSE);
}

static t_bool case_pipe_and_or(char **input_ptr, int *idx)
{
	char *input;

	input = *input_ptr;
	while ((9 <= input[++(*idx)] && input[(*idx)] <= 13) || input[(*idx)] == ' ')
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
		if (temp == T_NULL)
		{
			free(buffer);
			free(input);
			exit(1);
		}
		*input_ptr = temp;
		free(buffer);
		free(input);
		break;
	}
	return (TRUE);
}

static t_bool case_lts_gts(char *input, int *idx)
{
	int check;
	char *temp;

	while ((9 <= input[++(*idx)] && input[(*idx)] <= 13) || input[(*idx)] == ' ')
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
	check = (*idx)++;
	while (input[(*idx)] != '<' && input[(*idx)] != '>'
		&& input[(*idx)] != '\0' && input[(*idx)] != '|'
		&& !(input[(*idx)] == '&' && input[(*idx) + 1] == '&')
		&& !((9 <= input[(*idx)] && input[(*idx)] <= 13) || input[(*idx)] == ' '))
		(*idx)++;
	if (input[(*idx)] == '<' || input[(*idx)] == '>')
	{
		temp = ft_strndup(&input[check], (*idx) - check);
		if (temp == T_NULL)
		{
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
