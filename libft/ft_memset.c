/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:50 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:31:12 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			index;
	unsigned char	*b_char;
	unsigned char	c_char;

	index = 0;
	b_char = (unsigned char *)b;
	c_char = (unsigned char)c;
	while (index < len)
	{
		b_char[index] = c_char;
		index++;
	}
	return (b);
}
