/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:09:41 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:59:20 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int		count_pipe_in_tkns(char *tkns);
static char		**split_tkns_by_pipe(char *tkns, int pipe_cnt);
static t_bool	split_tkns_by_pipe2(char *tkns, int *idx, char **tmp);
static t_dll	**make_dlls(int pipe_cnt);

t_dll	**tokenize(char *tkns, int *pipe_cnt)
{
	char	**split_tmp;
	t_dll	**tkn_part;
	int		idx;

	(*pipe_cnt) = count_pipe_in_tkns(tkns) + 1;
	split_tmp = split_tkns_by_pipe(tkns, (*pipe_cnt));
	if (split_tmp == T_NULL)
		return (T_NULL);
	tkn_part = make_dlls((*pipe_cnt));
	if (tkn_part == T_NULL)
		return (free_2d_array((void ***)&split_tmp, (*pipe_cnt)));
	idx = -1;
	while (++idx < (*pipe_cnt))
	{
		if (redirect_split(tkn_part[idx], split_tmp[idx]) == FALSE)
			break ;
		if (parentheses_split(tkn_part[idx], split_tmp[idx]) == FALSE)
			break ;
		if (remain_split(tkn_part[idx], split_tmp[idx]) == FALSE)
			break ;
	}
	free_2d_array((void ***)&split_tmp, (*pipe_cnt));
	if (idx < (*pipe_cnt))
		return (free_2d_dll(&tkn_part, (*pipe_cnt), str_delete_func));
	return (tkn_part);
}

static int	count_pipe_in_tkns(char *tkns)
{
	int	idx;
	int	pipe_cnt;

	idx = -1;
	pipe_cnt = 0;
	while (tkns[++idx] != '\0')
	{
		if (tkns[idx] == '|')
			pipe_cnt++;
		else if (tkns[idx] == '\"' || tkns[idx] == '\'')
			ignore_quote(tkns, &idx);
		else if (tkns[idx] == '(')
			ignore_parentheses(tkns, &idx);
	}
	return (pipe_cnt);
}

static char	**split_tkns_by_pipe(char *tkns, int pipe_cnt)
{
	char	**tmp;
	int		idx[3];

	tmp = (char **)ft_calloc(pipe_cnt, sizeof(char *));
	if (tmp == T_NULL)
		return (T_NULL);
	idx[1] = 0;
	idx[2] = 0;
	idx[0] = -1;
	while (tkns[++idx[0]] != '\0')
		if (split_tkns_by_pipe2(tkns, idx, tmp) == FALSE)
			return (T_NULL);
	tmp[idx[2]] = ft_strndup(&tkns[idx[1]], idx[0] - idx[1]);
	if (tmp[idx[2]] == T_NULL)
		return (free_2d_array((void ***)&tmp, idx[2]));
	return (tmp);
}

static t_bool	split_tkns_by_pipe2(char *tkns, int *idx, char **tmp)
{
	if (tkns[idx[0]] == '|')
	{
		tmp[idx[2]] = ft_strndup(&tkns[idx[1]], idx[0] - idx[1]);
		if (tmp[idx[2]] == T_NULL)
		{
			free_2d_array((void ***)&tmp, idx[2]);
			return (FALSE);
		}
		idx[1] = idx[0] + 1;
		idx[2]++;
		idx[0]++;
	}
	else if (tkns[idx[0]] == '\"' || tkns[idx[0]] == '\'')
		ignore_quote(tkns, &idx[0]);
	else if (tkns[idx[0]] == '(')
		ignore_parentheses(tkns, &idx[0]);
	return (TRUE);
}

static t_dll	**make_dlls(int pipe_cnt)
{
	int		idx;
	t_dll	**tmp;

	tmp = (t_dll **)ft_calloc(pipe_cnt, sizeof(t_dll *));
	if (tmp == T_NULL)
		return (T_NULL);
	idx = -1;
	while (++idx < pipe_cnt)
	{
		tmp[idx] = (t_dll *)ft_calloc(1, sizeof(t_dll));
		if (tmp[idx] == T_NULL)
			return (free_2d_dll(&tmp, idx, T_NULL));
		dll_init(tmp[idx]);
		tmp[idx]->head.contents = ft_calloc(1, sizeof(t_cmd_info));
		if (tmp[idx]->head.contents == T_NULL)
		{
			ft_free1((void **)&tmp[idx]);
			return (free_2d_dll(&tmp, idx, T_NULL));
		}
		dll_init(&((t_cmd_info *)(tmp[idx]->head.contents))->heredoc_names);
	}
	return (tmp);
}
