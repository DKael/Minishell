/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 20:02:48 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:58:19 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static t_bool	oldpwd_exist(t_envval **tmp_env, char *path);
static t_bool	oldpwd_not_exist(t_envval **tmp_env, char *path);

t_bool	change_pwd_oldpwd(t_dll *env, char *path)
{
	t_dllnode	*ptr;
	t_envval	*tmp_env[3];

	tmp_env[1] = T_NULL;
	tmp_env[2] = T_NULL;
	ptr = env->head.back;
	while (ptr != &(env->tail))
	{
		tmp_env[0] = (t_envval *)(ptr->contents);
		if (ft_strcmp(tmp_env[0]->name, "PWD") == 0)
			tmp_env[1] = tmp_env[0];
		else if (ft_strcmp(tmp_env[0]->name, "OLDPWD") == 0)
			tmp_env[2] = tmp_env[0];
		ptr = ptr->back;
	}
	if (tmp_env[2] != T_NULL)
		return (oldpwd_exist(tmp_env, path));
	else
		return (oldpwd_not_exist(tmp_env, path));
}

static t_bool	oldpwd_exist(t_envval **tmp_env, char *path)
{
	if (tmp_env[1] == T_NULL)
	{
		ft_free1((void **)&tmp_env[2]->value);
		tmp_env[2]->value = ft_strdup("");
		if (tmp_env[2]->value == T_NULL)
			return (FALSE);
	}
	else
	{
		ft_free1((void **)&tmp_env[2]->value);
		tmp_env[2]->value = tmp_env[1]->value;
		tmp_env[1]->value = ft_strdup(path);
		if (tmp_env[1]->value == T_NULL)
			return (FALSE);
	}
	return (TRUE);
}

static t_bool	oldpwd_not_exist(t_envval **tmp_env, char *path)
{
	if (tmp_env[1] != T_NULL)
	{
		ft_free1((void **)&tmp_env[1]->value);
		tmp_env[1]->value = ft_strdup(path);
		if (tmp_env[1]->value == T_NULL)
			return (FALSE);
	}
	return (TRUE);
}
