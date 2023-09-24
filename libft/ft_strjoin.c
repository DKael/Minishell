/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:58 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:31 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	size_t	index;
	char	*result;

	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	result = (char *)malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (result == T_NULL)
		return (T_NULL);
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
