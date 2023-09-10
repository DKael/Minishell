/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 17:43:07 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/10 16:58:48 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	size_t	index;
	char	*result;

	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	result = (char *)malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (result == NULL)
		return (NULL);
	index = 0;
	while (index < s1_size)
	{
		result[index] = s1[index];
		index++;
	}
	index = 0;
	while (index < s2_size)
	{
		result[s1_size + index] = s2[index];
		index++;
	}
	result[s1_size + index] = '\0';
	return (result);
}

size_t	ft_strlen(const char *s)
{
	size_t	index;

	index = 0;
	while (s[index] != '\0')
	{
		index++;
	}
	return (index);
}

t_bool	ft_isdecimal(char *str)
{
	int	idx;

	idx = 0;
	if (str[0] == '-' || str[0] == '+')
		idx++;
	while (str[idx] != '\0')
	{
		if (!('0' <= str[idx] && str[idx] <= '9'))
			return (FALSE);
		idx++;
	}
	return (TRUE);
}

char	*ft_strndup(char *src, size_t n)
{
	size_t	index;
	char	*replica;

	replica = (char *)malloc(sizeof(char) * (n + 1));
	if (replica == NULL)
		return (NULL);
	index = 0;
	while (src[index] != '\0' && index < n)
	{
		replica[index] = src[index];
		index++;
	}
	replica[index] = '\0';
	return (replica);
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t	index;
	size_t	total_size;
	void	*result;
	char	*temp_ptr;

	index = 0;
	if (count == 0 || size == 0)
		return (malloc(0));
	total_size = count * size;
	if (ULLONG_MAX / size < count)
		return ((T_NULL));
	result = malloc(total_size);
	if (result == T_NULL)
		return (T_NULL);
	temp_ptr = (char *)result;
	while (index < total_size)
	{
		temp_ptr[index] = 0;
		index++;
	}
	return (result);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			index;
	unsigned char	*p1;
	unsigned char	*p2;

	index = 0;
	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (index < n)
	{
		if (p1[index] != p2[index])
			return ((int)(p1[index] - p2[index]));
		else if (p1[index] == '\0')
			return (0);
		index++;
	}
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int				index;
	unsigned char	*p1;
	unsigned char	*p2;

	index = 0;
	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (p1[index] == p2[index]
		&& p1[index] != '\0' && p2[index] != '\0')
		index++;
	return ((int)(p1[index] - p2[index]));
}

t_bool	ft_isblank(char c)
{
	if ((9 <= c && c < 13) || c == ' ')
		return (TRUE);
	return (FALSE);
}

t_bool	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (TRUE);
	return (FALSE);
}