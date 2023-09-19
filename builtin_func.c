/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 10:41:17 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/19 17:03:02 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **input)
{
	if (input == T_NULL)
		return (1);
	return (0);
}

int	ft_cd(char **input)
{
	if (input == T_NULL)
		return (1);
	return (0);
}

int	ft_export(char **input)
{
	if (input == T_NULL)
		return (1);
	return (0);
}

int	ft_unset(char **input)
{
	if (input == T_NULL)
		return (1);
	return (0);
}

int	ft_pwd(char **input)
{
	if (input == T_NULL)
		return (1);
	return (0);
}

int	ft_exit(char **input)
{
	if (input == T_NULL)
		return (1);
	return (0);
}