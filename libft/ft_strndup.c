/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:37:48 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:46 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *src, size_t n)
{
	size_t	index;
	char	*replica;

	replica = (char *)malloc(sizeof(char) * (n + 1));
	if (replica == T_NULL)
		return (T_NULL);
	index = 0;
	while (src[index] != '\0' && index < n)
	{
		replica[index] = src[index];
		index++;
	}
	replica[index] = '\0';
	return (replica);
}
