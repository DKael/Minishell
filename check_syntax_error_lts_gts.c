/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_lts_gts.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:16:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 02:34:20 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	case_lts_gts2(char *cmd, int *idx);
static t_bool	case_lts_gts3(char *cmd, int *idx, int check);

t_bool	meet_lts_gts(char *cmd, int *idx)
{
	if ((cmd[(*idx)] == '<' && cmd[(*idx) + 1] == '<')
		|| (cmd[(*idx)] == '>' && cmd[(*idx) + 1] == '>'))
		(*idx)++;
	if (case_lts_gts(cmd, idx) == FALSE)
		return (FALSE);
	return (TRUE);
}

t_bool	case_lts_gts(char *cmd, int *idx)
{
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
	return (case_lts_gts2(cmd, idx));
}

static t_bool	case_lts_gts2(char *cmd, int *idx)
{
	int	check;

	check = (*idx);
	while (cmd[++(*idx)] != '<' && cmd[(*idx)] != '>'
		&& cmd[(*idx)] != '\0' && cmd[(*idx)] != '|'
		&& !(cmd[(*idx)] == '&' && cmd[(*idx) + 1] == '&')
		&& cmd[(*idx)] != '(' && cmd[(*idx)] != ')'
		&& ft_isblank(cmd[(*idx)]) == FALSE)
		;
	return (case_lts_gts3(cmd, idx, check));
}

static t_bool	case_lts_gts3(char *cmd, int *idx, int check)
{
	char	*temp;

	if (cmd[(*idx)] == '<' || cmd[(*idx)] == '>')
	{
		temp = ft_strndup(&cmd[check], (*idx) - check);
		if (temp == T_NULL)
		{
			printf("minishell: malloc error!\n");
			ft_free1((void **)&cmd);
			exit(1);
		}
		if (ft_isdecimal(temp) == TRUE)
		{
			syntax_error_print(temp);
			ft_free1((void **)&temp);
			return (FALSE);
		}
		ft_free1((void **)&temp);
	}
	return (TRUE);
}
