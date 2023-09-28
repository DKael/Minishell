/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 21:40:27 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 14:33:11 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_envval	*make_envval_from_str(char *str);
static t_bool	find_env(t_dll *dll, t_dllnode	**ptr, t_envval *env,
					int *work);
static int		do_export(t_dll *dll, t_dll *s_env, char *str,
					t_envval *env);
static int		add_new(t_dllnode **ptr, t_dll *env, t_dll *s_env,
					t_envval *var);

int	ft_export(t_dll *env, t_dll *s_env, char **args)
{
	int			i;
	int			flag;
	int			result;
	t_envval	*tmp;

	if (args[1] == T_NULL)
	{
		dll_print(s_env, dll_export_print_func);
		return (0);
	}
	flag = 0;
	i = 0;
	while (args[++i])
	{
		tmp = make_envval_from_str(args[i]);
		if (tmp == T_NULL)
			return (-1);
		result = do_export(env, s_env, args[i], tmp);
		if (result == -1)
			return (-1);
		flag += result;
	}
	return (flag > 0);
}

static t_envval	*make_envval_from_str(char *str)
{
	int			idx;
	t_envval	*tmp;

	tmp = (t_envval *)ft_calloc(1, sizeof(t_envval));
	if (tmp == T_NULL)
		return (T_NULL);
	idx = -1;
	while (str[++idx] != '=' && str[idx] != '\0')
		;
	if (str[idx] == '=')
	{
		tmp->name = ft_strndup(str, idx);
		tmp->value = ft_strdup(&str[idx + 1]);
	}
	else
		tmp->name = ft_strdup(str);
	if (tmp->name == T_NULL || (str[idx] == '=' && tmp->value == T_NULL))
	{
		envval_delete_func((void *)tmp);
		return (T_NULL);
	}
	return (tmp);
}

static int	do_export(t_dll *dll, t_dll *s_env, char *str, t_envval *env)
{
	int			work;
	t_dllnode	*ptr[3];

	work = 0;
	if (ft_isalpha(str[0]) == FALSE && str[0] != '_')
	{
		err_msg_print3("export: ", str, ": not a valid identifier");
		envval_delete_func(env);
		return (1);
	}
	if (find_env(dll, ptr, env, &work) == FALSE)
		return (add_new(ptr, dll, s_env, env));
	else if (work == 0)
		envval_delete_func(env);
	return (0);
}

static t_bool	find_env(t_dll *dll, t_dllnode	**ptr, t_envval *env, int *work)
{
	t_envval	*tmp;

	ptr[0] = dll->head.back;
	while (ptr[0] != &(dll->tail))
	{
		tmp = ptr[0]->contents;
		if (ft_strcmp(tmp->name, env->name) == 0)
		{
			if (env->value != T_NULL)
			{
				free(tmp->value);
				tmp->value = env->value;
				free(env->name);
				free(env);
				(*work)++;
			}
			return (TRUE);
		}
		ptr[0] = ptr[0]->back;
	}
	return (FALSE);
}

static int	add_new(t_dllnode **ptr, t_dll *env, t_dll *s_env, t_envval *var)
{
	char	*t;

	ptr[2] = dll_new_node((void *)var);
	if (ptr[2] == T_NULL)
		return (-1);
	dll_add_tail(env, ptr[2]);
	ptr[1] = s_env->head.back;
	while (ptr[1] != &(s_env->tail))
	{
		t = ((t_envval *)(((t_dllnode *)(ptr[1]->contents))->contents))->name;
		if (ft_strcmp((char *)var->name, t) < 0)
		{
			if (dll_content_add2(s_env, (void *)ptr[2], ptr[1], 1) == FALSE)
				return (-1);
			else
				break ;
		}
		ptr[1] = ptr[1]->back;
	}
	if (ptr[1] == &(s_env->tail))
		if (dll_content_add2(s_env, (void *)ptr[2], ptr[1], 1) == FALSE)
			return (-1);
	return (0);
}
