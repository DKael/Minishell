/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 09:26:30 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/16 13:08:21 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	heredoc_make(t_dll *dll, char *del)
{
	t_stat bf;
	char	*make_path;

	make_path = ft_strdup(hd_path);
	if (make_path == T_NULL)
		return (FALSE);
	if (stat(hd_path, &bf) == -1 || !(((bf.st_mode) & S_IFMT) == S_IFDIR && ))
	{
		free(make_path);
		make_path = ft_strdup("./");
		if (make_path == T_NULL)
			return (FALSE);
	}
	
}