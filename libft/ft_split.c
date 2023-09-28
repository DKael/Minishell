/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:27:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 22:32:04 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		**do_split(char **result1, char *temp, size_t len);
static char		*do_copy(char *src, size_t *index);
static char		**do_remove(char **result2, size_t result_idx, char *temp);

char	**ft_split(char const *s, char c)
{
	char	*temp;
	char	**result;
	size_t	idx;
	size_t	count;

	temp = ft_strdup((char *)s);
	if (temp == T_NULL)
		return (T_NULL);
	idx = 0;
	count = 0;
	while (temp[idx] != '\0')
	{
		if (temp[idx] != c && (temp[idx + 1] == c || temp[idx + 1] == '\0'))
			count++;
		if (temp[idx] == c)
			temp[idx] = '\0';
		idx++;
	}
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (result == T_NULL)
	{
		free(temp);
		return (T_NULL);
	}
	return (do_split(result, temp, idx));
}

static char	**do_split(char **result1, char *temp, size_t len)
{
	size_t	idx;
	size_t	result_idx;

	idx = 0;
	result_idx = 0;
	if (temp[idx] != '\0')
	{
		result1[result_idx] = do_copy(temp, &idx);
		if (result1[result_idx] == T_NULL)
			return (do_remove(result1, result_idx, temp));
		result_idx++;
	}
	while (++idx < len)
	{
		if (temp[idx] != '\0' && temp[idx - 1] == '\0')
		{
			result1[result_idx] = do_copy(&temp[idx], &idx);
			if (result1[result_idx] == T_NULL)
				return (do_remove(result1, result_idx, temp));
			result_idx++;
		}
	}
	result1[result_idx] = T_NULL;
	free(temp);
	return (result1);
}

static char	*do_copy(char *src, size_t *src_index)
{
	size_t	src_length;
	size_t	idx;
	char	*replica;

	src_length = ft_strlen(src);
	replica = (char *)malloc(sizeof(char) * (src_length + 1));
	if (replica == T_NULL)
		return (T_NULL);
	idx = 0;
	while (src[idx] != '\0')
	{
		replica[idx] = src[idx];
		idx++;
	}
	replica[idx] = '\0';
	*src_index += src_length - 1;
	return (replica);
}

static char	**do_remove(char **result2, size_t result_idx, char *temp)
{
	size_t	index;

	index = 0;
	while (index < result_idx)
	{
		free(result2[index]);
		index++;
	}
	free(result2);
	free(temp);
	return (T_NULL);
}
