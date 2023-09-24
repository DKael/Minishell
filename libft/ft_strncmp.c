/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:28:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:43 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
