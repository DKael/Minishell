/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_parentheses2_bonus.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 01:35:18 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:58:28 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static t_bool	meet_something_behind_of_parentheses2(char *cmd,
					int *idx, int save_idx, t_bool *parentheses_flag);

t_bool	meet_something_behind_of_parentheses(char *cmd,
	int *idx, t_bool *parentheses_flag)
{
	int	save_idx;

	save_idx = (*idx);
	while (cmd[(*idx)] != '\0' && cmd[(*idx)] != '(' && cmd[(*idx)] != ')'
		&& ft_isblank(cmd[(*idx)]) == FALSE && cmd[(*idx)] != '|'
		&& cmd[(*idx)] != '&' && cmd[(*idx)] != '<' && cmd[(*idx)] != '>')
	{
		if (cmd[(*idx)] == '\'' || cmd[(*idx)] == '\"')
			ignore_quote(cmd, &(*idx));
		(*idx)++;
	}
	return (meet_something_behind_of_parentheses2(cmd, idx,
			save_idx, parentheses_flag));
}

static t_bool	meet_something_behind_of_parentheses2(char *cmd,
	int *idx, int save_idx, t_bool *parentheses_flag)
{
	if (cmd[(*idx)] == '<' || cmd[(*idx)] == '>')
	{
		if (ft_isndecimal(&cmd[save_idx], (*idx) - save_idx) == FALSE)
		{
			cmd[(*idx)] = '\0';
			return (syntax_error_print(&cmd[save_idx]));
		}
		if ((cmd[(*idx)] == '<' && cmd[(*idx) + 1] == '<')
			|| (cmd[(*idx)] == '>' && cmd[(*idx) + 1] == '>'))
			(*idx)++;
		if (case_lts_gts(cmd, &(*idx)) == FALSE)
			return (FALSE);
		(*idx)--;
	}
	else if ((cmd[(*idx)] == '|' || cmd[(*idx)] == '&') && save_idx == (*idx))
		(*parentheses_flag) = FALSE;
	else
	{
		cmd[(*idx)] = '\0';
		return (syntax_error_print(&cmd[save_idx]));
	}
	return (TRUE);
}
