/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:28:05 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:56 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		check_in_set(char c, char const *set);
static char		*do_trim(char const *s1, size_t front, size_t back);

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1_len;
	size_t	front;
	size_t	back;
	char	*result;

	s1_len = ft_strlen(s1);
	front = 0;
	while (check_in_set(s1[front], set) && s1[front] != '\0')
		front++;
	if (s1[front] == '\0')
	{
		result = (char *)malloc(sizeof(char) * 1);
		if (result == T_NULL)
			return (T_NULL);
		result[0] = '\0';
		return (result);
	}
	else
	{
		back = s1_len - 1;
		while (check_in_set(s1[back], set))
			back--;
		return (do_trim(s1, front, back));
	}
}

static char	*do_trim(char const *s1, size_t front, size_t back)
{
	char	*result;
	size_t	len;
	size_t	idx;

	len = back - front + 1;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (result == T_NULL)
		return (T_NULL);
	idx = 0;
	while (idx < len)
	{
		result[idx] = s1[front + idx];
		idx++;
	}
	result[idx] = '\0';
	return (result);
}

static int	check_in_set(char c, char const *set)
{
	int	idx;

	idx = 0;
	while (set[idx] != '\0')
	{
		if (set[idx] == c)
			return (1);
		idx++;
	}
	return (0);
}
