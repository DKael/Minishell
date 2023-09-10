/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 11:35:22 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/10 11:35:38 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool syntax_error_print(char *chr)
{
	printf("minishell: syntax error near unexpected token `%s'\n", chr);
	return (FALSE);
}