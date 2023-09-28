/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitpid_macro_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 18:01:57 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 21:53:43 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	ft_wifexited(int status)
{
	if (((*(int *)&(status)) & 0177) == 0)
		return (TRUE);
	else
		return (FALSE);
}

int	ft_wexitstatus(int status)
{
	return (((*(int *)&(status)) >> 8) & 0x000000ff);
}

t_bool	ft_wifsignaled(int status)
{
	if (((*(int *)&(status)) & 0177) != _WSTOPPED
		&& ((*(int *)&(status)) & 0177) != 0)
		return (TRUE);
	else
		return (FALSE);
}

int	ft_wtermsig(int status)
{
	return (((*(int *)&(status)) & 0177));
}
