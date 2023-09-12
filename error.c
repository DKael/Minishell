/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 11:35:22 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/12 11:34:57 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool syntax_error_print(char *chr)
{
	printf("minishell: syntax error near unexpected token `%s'\n", chr);
	return (FALSE);
}

void message_exit(const char *msg, int exit_code)
{
	unsigned int uexit_code;

	uexit_code = ((unsigned int)exit_code) % 256;
	write(2, msg, ft_strlen(msg));
	exit((int)uexit_code);
}