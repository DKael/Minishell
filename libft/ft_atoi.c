/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:21 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:53:33 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int			idx;
	int			sign;
	long long	nb;

	idx = 0;
	sign = 1;
	nb = 0;
	while ((9 <= str[idx] && str[idx] <= 13) || str[idx] == ' ')
		idx++;
	if (str[idx] == '-' || str[idx] == '+')
		if (str[idx++] == '-')
			sign = -1;
	while ('0' <= str[idx] && str[idx] <= '9')
	{
		if ((nb == LLONG_MAX / 10 && str[idx] - '0' > 7 + ((1 - sign) / 2))
			|| nb > LLONG_MAX / 10)
		{
			if (sign == -1)
				return (-1);
			else
				return (0);
		}
		nb = nb * 10 + (str[idx++] - '0');
	}
	return ((int)nb * sign);
}
