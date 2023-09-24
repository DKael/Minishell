/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:36 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:51:15 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*case_pos(int n, int digit);
static char	*case_neg(int n, int digit);

char	*ft_itoa(int n)
{
	int		digit;
	int		temp_n;
	char	*result;

	if (n == 0)
	{
		result = (char *)malloc(sizeof(char) * 2);
		if (result == T_NULL)
			return (T_NULL);
		result[0] = '0';
		result[1] = '\0';
		return (result);
	}
	temp_n = n;
	digit = 0;
	while (temp_n != 0)
	{
		digit++;
		temp_n /= 10;
	}
	if (n > 0)
		return (case_pos(n, digit));
	else
		return (case_neg(n, digit));
}

static char	*case_pos(int n, int digit)
{
	char	*result;

	result = (char *)malloc(sizeof(char) * (digit + 1));
	if (result == T_NULL)
		return (T_NULL);
	result[digit--] = '\0';
	while (n != 0)
	{
		result[digit--] = (n % 10) + '0';
		n /= 10;
	}
	return (result);
}

static char	*case_neg(int n, int digit)
{
	char	*result;

	result = (char *)malloc(sizeof(char) * (digit + 2));
	if (result == T_NULL)
		return (T_NULL);
	result[0] = '-';
	result[digit + 1] = '\0';
	while (n != 0)
	{
		result[digit--] = (-1) * (n % 10) + '0';
		n /= 10;
	}
	return (result);
}
