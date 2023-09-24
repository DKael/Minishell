/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:59 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:28:12 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	dst_length;
	size_t	src_length;
	size_t	src_index;

	dst_length = 0;
	if (dst != T_NULL)
		dst_length = ft_strlen(dst);
	src_length = ft_strlen(src);
	if (dsize < dst_length + 1)
		return (dsize + src_length);
	else
	{
		src_index = 0;
		while (src_index + dst_length + 1 < dsize && src[src_index] != '\0')
		{
			dst[dst_length + src_index] = src[src_index];
			src_index++;
		}
		dst[dst_length + src_index] = '\0';
		return (dst_length + src_length);
	}
}
