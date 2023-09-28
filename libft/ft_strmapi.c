/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:28:02 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	len_s;
	unsigned int	index;
	char			*result;

	len_s = 0;
	while (s[len_s] != '\0')
		len_s++;
	result = (char *)malloc(sizeof(char) * (len_s + 1));
	if (result == T_NULL)
		return (T_NULL);
	index = 0;
	while (s[index] != '\0')
	{
		result[index] = f(index, s[index]);
		index++;
	}
	result[index] = '\0';
	return (result);
}
