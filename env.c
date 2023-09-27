/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 21:15:49 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 18:19:03 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_data *data, char **input)
{
	int	idx;

	idx = -1;
	while (input[++idx] != T_NULL)
		;
	if (idx >= 2)
	{
		write(STDERR_FILENO, "env: ", 5);
		write(STDERR_FILENO, input[1], ft_strlen(input[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		return (127);
	}
	dll_print(&data->envdll, dll_env_print_func);
	return (0);
}
