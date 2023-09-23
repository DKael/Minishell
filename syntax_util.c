/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 21:31:45 by junehyle          #+#    #+#             */
/*   Updated: 2023/09/23 21:34:05 by junehyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	pipe_and_or(char *cmd, int idx)
{
	if (cmd[idx] == '|')
	{
		if (cmd[idx + 1] == '|')
			return (syntax_error_print("||"));
		else
			return (syntax_error_print("|"));
	}
	else if (cmd[idx] == '&' && cmd[idx + 1] == '&')
		return (syntax_error_print("&&"));
	return (TRUE);
}

t_bool	unexpected_eod_print(void)
{
	printf("minishell: syntax error: unexpected end of file\n");
	return (FALSE);
}
