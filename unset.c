/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:26:02 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 16:02:50 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_start_char_unset(char *str);

int	ft_unset(t_dll *env, t_dll *s_env, char **args)
{
	int	i;
	int	flag;

	flag = 0;
	i = 1;
	while (args[i])
	{
		flag += do_unset(s_env, env, args[i]);
		i++;
	}
	return (flag > 0);
}

static int	check_start_char_unset(char *str)
{
	if (!(str[0] >= 'a' && str[0] <= 'z') && !(str[0] >= 'A' && str[0] <= 'Z'))
	{
		err_msg_print3("unset: ", str, ": not a valid identifier");
		return (1);
	}
	return (0);
}

static int	do_unset(t_dll *s_env, t_dll *env, char *str)
{
	int			flag;
	t_dllnode	*dllnode;
	char		*tmp;

	flag = 0;
	if (s_env)
	{
		flag += check_start_char_unset(str);
		dllnode = s_env->head.back;
		while (dllnode != &(s_env->tail))
		{
			tmp = (char *)((t_envval *)((((t_dllnode *)
								(dllnode->contents))->contents)))->name;
			if (!(ft_strcmp(tmp, str)))
			{
				dll_del_node(env, dllnode->contents, envval_delete_func);
				dll_del_node(s_env, dllnode, T_NULL);
				break ;
			}
			dllnode = dllnode->back;
		}
	}
	return (flag);
}
