/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:51:41 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:07:15 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		remain_split2_1(t_cmd_info *tmp, int *idx, char *tkns);
static t_bool	remain_split2_2(t_dll *dll, char *tkns, int *idx);
static void		remain_split2_3(char *tkns, int *idx, int *idx2, char *tmp);

t_bool	remain_split(t_dll *dll, char *tkns)
{
	int			idx[3];
	t_cmd_info	*tmp;

	tmp = (t_cmd_info *)(dll->head.contents);
	idx[0] = 0;
	while (tkns[idx[0]] != '\0')
	{
		if (ft_isblank(tkns[idx[0]]) == FALSE)
		{
			remain_split2_1(tmp, idx, tkns);
			if (remain_split2_2(dll, tkns, idx) == FALSE)
				return (FALSE);
		}
		else
			idx[0]++;
	}
	tmp->size = tmp->redir_cnt + tmp->cp_cnt;
	return (TRUE);
}

static void	remain_split2_1(t_cmd_info *tmp, int *idx, char *tkns)
{
	tmp->cp_cnt++;
	idx[2] = 0;
	idx[1] = idx[0];
	while (ft_isblank(tkns[idx[0]]) == FALSE && tkns[idx[0]] != '\0')
	{
		if (tkns[idx[0]] == '\"' || tkns[idx[0]] == '\'')
		{
			idx[2] += 2;
			ignore_quote(tkns, &idx[0]);
		}
		idx[0]++;
	}
}

static t_bool	remain_split2_2(t_dll *dll, char *tkns, int *idx)
{
	char	*tmp;
	int		idx2;

	tmp = (char *)ft_calloc(idx[0] - idx[1] - idx[2] + 1, sizeof(char));
	if (tmp == T_NULL)
		return (FALSE);
	idx2 = -1;
	while (idx[1] < idx[0])
		remain_split2_3(tkns, idx, &idx2, tmp);
	if (dll_content_add(dll, (void *)tmp, 0) == FALSE)
		return (ft_free2((void **)&tmp, FALSE));
	return (TRUE);
}

static void	remain_split2_3(char *tkns, int *idx, int *idx2, char *tmp)
{
	char	char_tmp;

	if (tkns[idx[1]] == '\'' || tkns[idx[1]] == '\"')
	{
		char_tmp = tkns[idx[1]];
		while (tkns[++idx[1]] != char_tmp)
		{
			if (tkns[idx[1]] == '$' && char_tmp == '\"')
				tkns[idx[1]] = -1;
			tmp[++(*idx2)] = tkns[idx[1]];
		}
	}
	else
	{
		if (tkns[idx[1]] == '$')
			tkns[idx[1]] = -1;
		tmp[++(*idx2)] = tkns[idx[1]];
	}
	idx[1]++;
}
