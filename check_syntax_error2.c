/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 19:22:13 by junehyle          #+#    #+#             */
/*   Updated: 2023/09/23 19:36:43 by junehyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	comb_redirect(char *cmd, int *idx)//case_lts_gts에 주석처리된 윗부분
{
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
	return (TRUE);
}

t_bool	check_redirect(char *cmd, char *temp)//case_lts_gts에 주석 처리된 아랫부분
{
	if (temp == T_NULL)
	{
		printf("minishell: malloc error!\n");
		ft_free1((void **)cmd);
		exit(1);
	}
	if (ft_isdecimal(temp) == TRUE)
	{
		syntax_error_print(temp);
		ft_free1((void **)temp);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	case_lts_gts(char *cmd, int *idx)
{
	int		check;
	char	*temp;

	while (ft_isblank(cmd[++(*idx)]) == TRUE)
		;
	if (comb_redirect(cmd, idx) != TRUE)
		return (FALSE);
/*	if (cmd[(*idx)] == '>' && cmd[(*idx) + 1] != '>')
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
		return (syntax_error_print("newline"));*/
	check = (*idx);
	while (cmd[++(*idx)] != '<' && cmd[(*idx)] != '>'
		&& cmd[(*idx)] != '\0' && cmd[(*idx)] != '|'
		&& !(cmd[(*idx)] == '&' && cmd[(*idx) + 1] == '&')
		&& cmd[(*idx)] != '(' && cmd[(*idx)] != ')'
		&& ft_isblank(cmd[(*idx)]) == FALSE)
		;
	if (cmd[(*idx)] == '<' || cmd[(*idx)] == '>')
	{
		temp = ft_strndup(&cmd[check], (*idx) - check);
		if (check_redirect(cmd, temp) == FALSE)
			return (FALSE);
		else
			ft_free1((void **)temp);
		/*if (temp == T_NULL)
		{
			printf("minishell: malloc error!\n");
			ft_free1((void **)cmd);
			exit(1);
		}
		if (ft_isdecimal(temp) == TRUE)
		{
			syntax_error_print(temp);
			ft_free1((void **)temp);
			return (FALSE);
		}*/
	}
	return (TRUE);
}
