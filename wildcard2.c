/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 17:59:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 19:55:36 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	do_convert(t_dll *dll, char *tkn);

int	wildcard_redirection_check(t_dll *dll, t_dll *tmp_dll, t_dllnode **ptr)
{
	int		idx[2];
	char	*tkn;
	int		result;

	idx[0] = -1;
	while (++idx[0] < ((t_cmd_info *)(dll->head.contents))->redir_cnt)
	{
		tkn = (char *)(*ptr)->contents;
		if (ft_strstr(tkn, "*") != T_NULL && ft_strstr(tkn, "<<") == T_NULL)
		{
			tkn = ft_strstr(tkn, " ");
			result = do_convert(tmp_dll, ++tkn) == -1;
			if (result != 0)
				return (result);
			if (tmp_dll->size != 0)
			{
				err_msg_print2(tkn, ": ambiguous redirect");
				dll_clear(tmp_dll, str_delete_func);
				return (1);
			}
		}
		(*ptr) = (*ptr)->back;
	}
	return (0);
}

char	*remove_duplicate_asterisk_slash(char *str)
{
	char	*tmp;
	int		idx1;
	int		idx2;

	tmp = ft_strdup(str);
	if (tmp == T_NULL)
		return (T_NULL);
	idx1 = -1;
	idx2 = -1;
	while (str[++idx1] != '\0')
	{
		if (str[idx1] == '*' || str[idx1] == '/')
		{
			tmp[++idx2] = str[idx1];
			while (str[++idx1] == tmp[idx2])
				;
			idx1--;
		}
		else
			tmp[++idx2] = str[idx1];
	}
	tmp[++idx2] = '\0';
	return (tmp);
}
