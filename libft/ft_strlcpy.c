/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:28:00 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:36 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	index;
	size_t	index1;

	index = 0;
	index1 = -1;
	while (src[index] != '\0')
		index++;
	if (index >= dsize)
	{
		while (++index1 + 1 < dsize)
			dst[index1] = src[index1];
	}
	else
	{
		while (src[++index1] != '\0')
			dst[index1] = src[index1];
	}
	if (dsize != 0)
		dst[index1] = '\0';
	return (index);
}
