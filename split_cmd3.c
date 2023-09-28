/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:51:15 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 20:05:34 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parentheses_split2(char *tkns, int *i);
extern void	find_front(char *tkns, int *pos, int idx);
extern void	find_back_and_calc_blank_quote(char *tkns, int *pos, int idx);
extern char	*redirect_split2_0(char *tkns, t_cmd_info *tmp,
				int *pos, t_bool heredoc_flag);

t_bool	redirect_split(t_dll *dll, char *tkns)
{
	int			pos[5];
	char		*con;
	t_cmd_info	*tmp;
	t_bool		heredoc_flag;

	heredoc_flag = FALSE;
	tmp = (t_cmd_info *)(dll->head.contents);
	pos[4] = -1;
	while (tkns[++pos[4]] != '\0')
	{
		if (tkns[pos[4]] == '<' || tkns[pos[4]] == '>')
		{
			con = redirect_split2_0(tkns, tmp, pos, heredoc_flag);
			if (con == T_NULL)
				return (FALSE);
			if (dll_content_add(dll, (void *)con, 0) == FALSE)
				return (ft_free2((void **)&con, FALSE));
			pos[4] = pos[1] - 1;
		}
		else if (tkns[pos[4]] == '\"' || tkns[pos[4]] == '\'')
			ignore_quote(tkns, &pos[4]);
		else if (tkns[pos[4]] == '(')
			ignore_parentheses(tkns, &pos[4]);
	}
	return (TRUE);
}

t_bool	parentheses_split(t_dll *dll, char *tkns)
{
	int		i[3];
	char	*tmp;

	i[0] = -1;
	i[2] = 0;
	while (tkns[++i[0]] != '\0')
	{
		if (tkns[i[0]] == '(')
		{
			((t_cmd_info *)(dll->head.contents))->parentheses_flag = TRUE;
			i[1] = i[0];
			parentheses_split2(tkns, i);
			tmp = ft_strndup(&tkns[i[1]], i[0] - i[1] + 1);
			if (tmp == T_NULL)
				return (FALSE);
			if (dll_content_add(dll, (void *)tmp, 0) == FALSE)
				return (ft_free2((void **)&tmp, FALSE));
			while (i[1] <= i[0])
				tkns[i[1]++] = ' ';
		}
	}
	return (TRUE);
}

static void	parentheses_split2(char *tkns, int *i)
{
	while (1)
	{
		i[0]++;
		if (tkns[i[0]] == ')')
		{
			if (i[2] == 0)
				break ;
			else
				i[2]--;
		}
		else if (tkns[i[0]] == '(')
			i[2]++;
		else if (tkns[i[0]] == '\'' || tkns[i[0]] == '\"')
			ignore_quote(tkns, &i[0]);
	}
}
