/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 16:28:59 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/18 17:19:32 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	child(t_data *data, int ao_idx, int pp_idx)
{
	t_dll	*tmp;

	tmp = data->tkn[ao_idx][pp_idx];

	
	sem_wait(data->print_sem);
	printf("\n-----------------------------------------------------\n");
	printf("\t<%d-%d child process report>\n", ao_idx, pp_idx);
	dll_print(tmp, dll_str_print_func);
	sem_post(data->print_sem);
}