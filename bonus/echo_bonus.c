/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 21:15:42 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:58:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	print_str(char **str, int i);

int	ft_echo(char **str)
{
	if (str[1] == T_NULL)
		printf("\n");
	else if (!(ft_strcmp(str[1], "-n")))
		print_str(str, 2);
	else
		print_str(str, 1);
	return (0);
}

static void	print_str(char **str, int i)
{
	if (i == 1)
	{
		while (str[i])
		{
			printf("%s", str[i++]);
			if (str[i])
				printf(" ");
		}
		printf("\n");
	}
	else
	{
		while (str[i])
		{
			printf("%s", str[i++]);
			if (str[i])
				printf(" ");
		}
	}
}
