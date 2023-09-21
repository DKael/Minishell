#include "minishell.h"


t_envval *make_envval_from_str(char *str)
{
	int	idx;
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

int	check_start_char_export(char *str)
{
	if (!(str[0] >= 'a' && str[0] <= 'z') && !(str[0] >= 'A' && str[0] <= 'Z'))
	{
		err_msg_print3("export: ", str, ": not a valid identifier");
		return (1);
	}
	return(0);
}

int	do_export(t_dll *dll, t_dll *sorted_env, char *str, t_envval *env)
{
	int	flag;
	int	work;
	t_dllnode	*dllnode;
	t_dllnode	*dllnode1;
	t_envval	*tmp;
	t_dllnode	*ptr;

	flag = 0;
	work = 0;
	
	if (check_start_char_export(str) == 1)
	{
		envval_delete_func(env);
		return (1);
	}
		
	dllnode = dll->head.back;
	while (dllnode != &(dll->tail))
	{
		tmp = dllnode->contents;
		if (ft_strcmp(tmp->name, env->name) == 0)
		{
			if (env->value != T_NULL)
			{
				free(tmp->value);
				tmp->value = env->value;
				free(env->name);
				free(env);
				work++;
			}
			break ;
		}
		dllnode = dllnode->back;
	}
	if (dllnode == &(dll->tail))
	{
		ptr = dll_new_node((void *)env);
		if (ptr == T_NULL)
			return (-1);
		dll_add_tail(dll, ptr);
		dllnode1 = sorted_env->head.back;
		while (dllnode1 != &(sorted_env->tail))
		{
			if (ft_strcmp((char *)env->name, (((t_envval *)(((t_dllnode*)(dllnode1->contents))->contents))->name)) < 0)
			{
				if (dll_content_add2(sorted_env, (void *)ptr, dllnode1, 1) == FALSE)
					return (-1);
				else
					break;
			}
			dllnode1 = dllnode1->back;
		}
		if (dllnode1 == &(sorted_env->tail))
			if (dll_content_add2(sorted_env, (void *)ptr, dllnode1, 1) == FALSE)
				return (-1);
	}
	else if (work == 0)
		envval_delete_func(env);
	return (flag);
}

int	ft_export(t_dll *env, t_dll *s_env, char **args)
{
	int	i;
	int	flag;
	int	result;
	t_envval *tmp;

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