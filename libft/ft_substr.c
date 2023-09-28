/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:28:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:59 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	idx;
	size_t	s_len;

	s_len = ft_strlen(s);
	if ((size_t)start < s_len && (size_t)start + len > s_len)
		result = (char *)malloc(sizeof(char) * (s_len - (size_t)start + 1));
	else if ((size_t)start < s_len && (size_t)start + len <= s_len)
		result = (char *)malloc(sizeof(char) * (len + 1));
	else
		result = (char *)malloc(sizeof(char) * 1);
	if (result == T_NULL)
		return (T_NULL);
	idx = 0;
	if (s_len > (size_t)start)
	{
		while (idx < len && s[(size_t)start + idx] != '\0')
		{
			result[idx] = s[(size_t)start + idx];
			idx++;
		}
	}
	result[idx] = '\0';
	return (result);
}
