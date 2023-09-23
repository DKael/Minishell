/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:31:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/23 16:58:03 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ft_strdup(char *src)
{
	size_t	src_length;
	size_t	index;
	char	*replica;

	src_length = ft_strlen(src);
	replica = (char *)malloc(sizeof(char) * (src_length + 1));
	if (replica == T_NULL)
		return (T_NULL);
	index = 0;
	while (src[index] != '\0')
	{
		replica[index] = src[index];
		index++;
	}
	replica[index] = '\0';
	return (replica);
}

long long	ft_atoll(const char *str)
{
	long long	sign;
	long long	nb;
	int	idx;

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
		if (nb > 922337203685477580
			|| (nb == 922337203685477580 && str[idx] - '0' > 7 + ((1 - sign) / 2)))
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

char	*ft_strstr(char *str, char *to_find)
{
	int	str_index;
	int	find_index;

	if (to_find[0] == '\0')
		return (str);
	str_index = 0;
	while (str[str_index] != '\0')
	{
		if (str[str_index] == to_find[0])
		{
			find_index = 1;
			while (to_find[find_index] != '\0')
			{
				if (str[str_index + find_index] != to_find[find_index])
					break ;
				find_index++;
			}
			if (to_find[find_index] == '\0')
				return (&str[str_index]);
		}
		str_index++;
	}
	return (T_NULL);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	index;
	size_t	index1;

	index = 0;
	index1 = -1;
	while (src[index] != '\0')
		index++;
	if (index >= dsize)
	{
		while (++index1 + 1 < dsize)
			dst[index1] = src[index1];
	}
	else
	{
		while (src[++index1] != '\0')
			dst[index1] = src[index1];
	}
	if (dsize != 0)
		dst[index1] = '\0';
	return (index);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	index;

	index = 0;
	while (src[index] != '\0')
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
	return (dest);
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	index;

	index = 0;
	while (index < n && src[index] != '\0')
	{
		dest[index] = src[index];
		index++;
	}
	if (index < n)
	{
		while (index < n)
		{
			dest[index] = '\0';
			index++;
		}
	}
	return (dest);
}