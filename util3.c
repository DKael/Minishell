/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:32:02 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/20 00:08:49 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char		**do_split(char **result1, char *temp, size_t len);
static char		*do_copy(char *src, size_t *index);
static char		**do_remove(char **result2, size_t result_idx, char *temp);

char	**ft_split(char const *s, char c)
{
	char	*temp;
	char	**result;
	size_t	idx;
	size_t	count;

	temp = ft_strdup((char *)s);
	if (temp == T_NULL)
		return (T_NULL);
	idx = 0;
	count = 0;
	while (temp[idx] != '\0')
	{
		if (temp[idx] != c && (temp[idx + 1] == c || temp[idx + 1] == '\0'))
			count++;
		if (temp[idx] == c)
			temp[idx] = '\0';
		idx++;
	}
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (result == T_NULL)
	{
		ft_free1((void **)&temp);
		return (T_NULL);
	}
	return (do_split(result, temp, idx));
}

static char	**do_split(char **result1, char *temp, size_t len)
{
	size_t	idx;
	size_t	result_idx;

	idx = 0;
	result_idx = 0;
	if (temp[idx] != '\0')
	{
		result1[result_idx] = do_copy(temp, &idx);
		if (result1[result_idx] == T_NULL)
			return (do_remove(result1, result_idx, temp));
		result_idx++;
	}
	while (++idx < len)
	{
		if (temp[idx] != '\0' && temp[idx - 1] == '\0')
		{
			result1[result_idx] = do_copy(&temp[idx], &idx);
			if (result1[result_idx] == T_NULL)
				return (do_remove(result1, result_idx, temp));
			result_idx++;
		}
	}
	result1[result_idx] = T_NULL;
	ft_free1((void **)&temp);
	return (result1);
}

static char	*do_copy(char *src, size_t *src_index)
{
	size_t	src_length;
	size_t	idx;
	char	*replica;

	src_length = ft_strlen(src);
	replica = (char *)malloc(sizeof(char) * (src_length + 1));
	if (replica == T_NULL)
		return (T_NULL);
	idx = 0;
	while (src[idx] != '\0')
	{
		replica[idx] = src[idx];
		idx++;
	}
	replica[idx] = '\0';
	*src_index += src_length - 1;
	return (replica);
}

static char	**do_remove(char **result2, size_t result_idx, char *temp)
{
	size_t	index;

	index = 0;
	while (index < result_idx)
	{
		ft_free1((void **)&result2[index]);
		index++;
	}
	ft_free1((void **)&result2);
	ft_free1((void **)&temp);
	return (T_NULL);
}

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