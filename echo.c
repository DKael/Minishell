/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 14:28:56 by junehyle          #+#    #+#             */
/*   Updated: 2023/09/23 14:31:04 by junehyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_str(char **str, int i)
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

int	ft_echo(char **str)
{
	if (!(ft_strcmp(str[1], "-n")))
		print_str(str, 2);
	else
		print_str(str, 1);
	return (0);
}
