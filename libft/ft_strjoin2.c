/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:36:17 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:08:51 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin2(char const *s1, char const *s2, char *between)
{
	size_t	s1_size;
	size_t	s2_size;
	size_t	between_size;
	size_t	index;
	char	*result;

	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	between_size = ft_strlen(between);
	result = (char *)malloc(sizeof(char)
			* (s1_size + s2_size + between_size + 1));
	if (result == T_NULL)
		return (T_NULL);
	index = -1;
	while (++index < s1_size)
		result[index] = s1[index];
	index = -1;
	while (++index < between_size)
		result[s1_size + index] = between[index];
	index = -1;
	while (++index < s2_size)
		result[s1_size + between_size + index] = s2[index];
	result[s1_size + between_size + index] = '\0';
	return (result);
}
