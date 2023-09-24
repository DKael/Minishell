/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_dollor_braces.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:13:46 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 02:33:48 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	check_dollor_braces2(char *cmd, int *idx, int idx_chk);

t_bool	check_dollor_braces(char *cmd)
{
	int	idx;
	int	idx_chk;

	idx = -1;
	while (cmd[++idx] != '\0')
	{
		if (cmd[idx] == '$' && cmd[idx + 1] == '{')
		{
			idx_chk = idx;
			idx++;
			if (check_dollor_braces2(cmd, &idx, idx_chk) == FALSE)
				return (FALSE);
		}
	}
	return (TRUE);
}

static t_bool	check_dollor_braces2(char *cmd, int *idx, int idx_chk)
{
	t_bool	bad_flag;

	bad_flag = FALSE;
	while (cmd[++(*idx)] != '}')
	{
		if (ft_isblank(cmd[(*idx)]) == TRUE || cmd[(*idx)] == '$')
			bad_flag = TRUE;
		else if (cmd[(*idx)] == '\"' || cmd[(*idx)] == '\'')
		{
			bad_flag = TRUE;
			ignore_parentheses(cmd, idx);
		}
		else if (cmd[(*idx)] == '\0')
		{
			write(2, "minishell: syntax error, unclosed brace\n", 41);
			return (FALSE);
		}
	}
	if (bad_flag || idx - idx_chk == 2)
	{
		cmd[(*idx) + 1] = '\0';
		err_msg_print2(&cmd[idx_chk], ": bad substitution");
		return (FALSE);
	}
	return (TRUE);
}
