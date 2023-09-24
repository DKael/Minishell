/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:48 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:31:07 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			index;
	unsigned char	*dst_char;
	unsigned char	*src_char;

	if (dst != T_NULL || src != T_NULL)
	{
		index = 0;
		dst_char = (unsigned char *)dst;
		src_char = (unsigned char *)src;
		while (index < n)
		{
			dst_char[index] = src_char[index];
			index++;
		}
	}
	return (dst);
}
