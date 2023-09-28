/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isndecimal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:37:41 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:37:53 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool	ft_isndecimal(char *str, int n)
{
	int	idx;

	idx = 0;
	if (str[0] == '-' || str[0] == '+')
		idx++;
	while (str[idx] != '\0' && idx < n)
	{
		if (!('0' <= str[idx] && str[idx] <= '9'))
			return (FALSE);
		idx++;
	}
	return (TRUE);
}
