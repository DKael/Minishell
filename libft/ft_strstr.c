/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:54:02 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:54:12 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	str_index;
	int	find_index;

	if (to_find[0] == '\0')
		return (str);
	str_index = 0;
	while (str[str_index] != '\0')
	{
		if (str[str_index] == to_find[0])
		{
			find_index = 1;
			while (to_find[find_index] != '\0')
			{
				if (str[str_index + find_index] != to_find[find_index])
					break ;
				find_index++;
			}
			if (to_find[find_index] == '\0')
				return (&str[str_index]);
		}
		str_index++;
	}
	return (T_NULL);
}
