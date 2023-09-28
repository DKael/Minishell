/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:47 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:31:03 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			index;
	unsigned char	*s1_cptr;
	unsigned char	*s2_cptr;

	index = -1;
	s1_cptr = (unsigned char *)s1;
	s2_cptr = (unsigned char *)s2;
	while (++index < n)
	{
		if (s1_cptr[index] != s2_cptr[index])
			return ((int)(s1_cptr[index] - s2_cptr[index]));
	}
	return (0);
}
