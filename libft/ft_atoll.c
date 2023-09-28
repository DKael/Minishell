/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:51:38 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:09:00 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atoll(const char *str)
{
	long long	sign;
	long long	nb;
	int			idx;

	nb = 0;
	sign = 1;
	idx = 0;
	while ((9 <= str[idx] && str[idx] <= 13) || str[idx] == ' ')
		idx++;
	if (str[idx] == '+' || str[idx] == '-')
		if (str[idx++] == '-')
			sign = -1;
	while (str[idx] >= '0' && str[idx] <= '9')
	{
		if ((nb == LLONG_MAX / 10 && str[idx] - '0' > 7 + ((1 - sign) / 2))
			|| nb > LLONG_MAX / 10)
		{
			if (sign == -1)
				return (-9223372036854775807 - 1);
			else
				return (9223372036854775807);
		}
		nb = nb * 10 + (str[idx++] - '0');
	}
	return (nb * sign);
}
