/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:28 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:30:35 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			index;
	unsigned char	*b_char;

	index = 0;
	b_char = (unsigned char *)s;
	while (index < n)
	{
		b_char[index] = '\0';
		index++;
	}
}
