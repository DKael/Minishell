/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:52 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:31:38 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	case_pos(int n, int digit, int fd);
static void	case_neg(int n, int digit, int fd);

void	ft_putnbr_fd(int n, int fd)
{
	int		digit;
	int		temp_n;

	temp_n = n;
	digit = 0;
	while (temp_n != 0)
	{
		digit++;
		temp_n /= 10;
	}
	if (n == 0)
		write(fd, "0", sizeof(char));
	else if (n > 0)
		case_pos(n, digit, fd);
	else
		case_neg(n, digit, fd);
}

static void	case_pos(int n, int digit, int fd)
{
	char	result[20];
	int		save_digit;

	save_digit = digit;
	while (n != 0)
	{
		result[--digit] = (n % 10) + '0';
		n /= 10;
	}
	write(fd, result, sizeof(char) * save_digit);
}

static void	case_neg(int n, int digit, int fd)
{
	char	result[20];
	int		save_digit;

	save_digit = digit;
	while (n != 0)
	{
		result[digit--] = ((-1) * (n % 10)) + '0';
		n /= 10;
	}
	result[digit] = '-';
	write(fd, result, sizeof(char) * (save_digit + 1));
}
