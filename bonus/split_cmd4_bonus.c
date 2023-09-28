/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd4_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:50:15 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:59:23 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	redirect_split2_2(char *tkns, char *tmp, int *pos,
				t_bool heredoc_flag);

// pos[0] = front
// pos[1] = back
// pos[2] = blank
// pos[3] = quote_cnt
void	find_front(char *tkns, int *pos, int idx)
{
	pos[0] = idx;
	while (--pos[0] >= 0 && ft_isblank(tkns[pos[0]]) == FALSE
		&& ('0' <= tkns[pos[0]] && tkns[pos[0]] < '9'))
		;
	if (pos[0] >= 0 && ft_isblank(tkns[pos[0]]) == FALSE)
		pos[0] = idx;
	else
		pos[0]++;
}

// pos[0] = front
// pos[1] = back
// pos[2] = blank
// pos[3] = quote_cnt
void	find_back_and_calc_blank_quote(char *tkns, int *pos, int idx)
{
	char	char_tmp;

	pos[1] = idx;
	pos[2] = 0;
	pos[3] = 0;
	if (tkns[idx + 1] == '<' || tkns[idx + 1] == '>')
		pos[1]++;
	while (ft_isblank(tkns[++pos[1]]) == TRUE)
		pos[2]++;
	while (tkns[pos[1]] != '\0' && ft_isblank(tkns[pos[1]]) == FALSE
		&& tkns[pos[1]] != '<' && tkns[pos[1]] != '>')
	{
		if (tkns[pos[1]] == '\"' || tkns[pos[1]] == '\'')
		{
			pos[3] += 2;
			char_tmp = tkns[pos[1]];
			while (tkns[++pos[1]] != char_tmp)
				;
		}
		pos[1]++;
	}
}

// pos[0] = front
// pos[1] = back
// pos[2] = blank
// pos[3] = quote_cnt
char	*redirect_split2_0(char *tkns, t_cmd_info *tmp, int *pos,
	t_bool heredoc_flag)
{
	char	*con;

	if (tkns[pos[4] + 1] == '<')
		heredoc_flag = TRUE;
	else
		heredoc_flag = FALSE;
	tmp->redir_cnt++;
	find_front(tkns, pos, pos[4]);
	find_back_and_calc_blank_quote(tkns, pos, pos[4]);
	con = (char *)ft_calloc(pos[1] - pos[0] - pos[2]
			- pos[3] + 2, sizeof(char));
	if (con == T_NULL)
		return (T_NULL);
	redirect_split2_1(tkns, con, pos, heredoc_flag);
	return (con);
}

void	redirect_split2_1(char *tkns, char *tmp, int *pos,
	t_bool heredoc_flag)
{
	char	char_tmp;
	int		idx_chk;

	idx_chk = pos[0];
	pos[2] = -1;
	while (pos[0] < pos[1])
	{
		if (tkns[pos[0]] == '\"' || tkns[pos[0]] == '\'')
		{
			char_tmp = tkns[pos[0]];
			while (tkns[++pos[0]] != char_tmp)
			{
				if (tkns[pos[0]] == '$' && char_tmp == '\"' && !heredoc_flag)
					tkns[pos[0]] = -1;
				tmp[++pos[2]] = tkns[pos[0]];
			}
		}
		else if (ft_isblank(tkns[pos[0]]) == FALSE)
			redirect_split2_2(tkns, tmp, pos, heredoc_flag);
		pos[0]++;
	}
	while (idx_chk < pos[1])
		tkns[idx_chk++] = ' ';
}

static void	redirect_split2_2(char *tkns, char *tmp, int *pos,
	t_bool heredoc_flag)
{
	if (tkns[pos[0]] == '$' && !heredoc_flag)
		tkns[pos[0]] = -1;
	tmp[++pos[2]] = tkns[pos[0]];
	if (tkns[pos[0]] == '<' || tkns[pos[0]] == '>')
	{
		if (tkns[pos[0] + 1] == '<' || tkns[pos[0] + 1] == '>')
			tmp[++pos[2]] = tkns[++pos[0]];
		tmp[++pos[2]] = ' ';
	}
}
