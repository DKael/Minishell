/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:28:04 by hyungdki          #+#    #+#             */
/*   Updated: 2023/05/30 14:37:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int		index;

	index = 0;
	while (s[index] != '\0')
		index++;
	while (index >= 0)
	{
		if (s[index] == (char)c)
			return ((char *)s + index);
		index--;
	}
	return ((void *)0);
}
