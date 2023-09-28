/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_argument_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:01:49 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 09:40:39 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dll_env_print_func(void *content)
{
	t_envval	*tmp;

	tmp = (t_envval *)content;
	if (tmp != T_NULL && tmp->value != T_NULL)
		printf("%s=%s\n", tmp->name, tmp->value);
}

void	dll_export_print_func(void *content)
{
	t_dllnode	*tmp1;
	t_envval	*tmp2;

	tmp1 = (t_dllnode *)content;
	tmp2 = (t_envval *)(tmp1->contents);
	if (tmp2 != T_NULL)
	{
		if (tmp2->value != T_NULL)
			printf("%s=\"%s\"\n", tmp2->name, tmp2->value);
		else
			printf("%s\n", tmp2->name);
	}
}

void	dll_str_print_func(void *content)
{
	char	*tmp;

	tmp = (char *)content;
	printf("%s\n", tmp);
}
