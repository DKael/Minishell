/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:28:17 by hyungdki          #+#    #+#             */
/*   Updated: 2023/05/08 14:13:54 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(const char *s1, const char *s2)
{
	int				index;
	unsigned char	*p1;
	unsigned char	*p2;

	index = 0;
	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (p1[index] == p2[index]
		&& p1[index] != '\0' && p2[index] != '\0')
		index++;
	return ((int)(p1[index] - p2[index]));
}
