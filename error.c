/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 11:35:22 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 21:58:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool syntax_error_print(char *chr)
{
	err_msg_print3("syntax error near unexpected token `", chr, "'\n");
	return (FALSE);
}

void	err_msg_print1(char *m1)
{
	if (m1 != T_NULL)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, m1, ft_strlen(m1));
		write(STDERR_FILENO, "\n", 1);
	}
	
}

void	err_msg_print2(char *m1, char *m2)
{
	if (m1 != T_NULL && m2 != T_NULL)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, m1, ft_strlen(m1));
		write(STDERR_FILENO, m2, ft_strlen(m2));
		write(STDERR_FILENO, "\n", 1);
	}
}

void	err_msg_print3(char *m1, char *m2, char *m3)
{
	if (m1 != T_NULL && m2 != T_NULL && m3 != T_NULL)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, m1, ft_strlen(m1));
		write(STDERR_FILENO, m2, ft_strlen(m2));
		write(STDERR_FILENO, m3, ft_strlen(m3));
		write(STDERR_FILENO, "\n", 1);
	}
}

void message_exit(const char *msg, int exit_code)
{
	exit_code = exit_code % 256;
	write(STDERR_FILENO, msg, ft_strlen(msg));
	exit(exit_code);
}