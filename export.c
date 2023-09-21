#include "minishell.h"

int	find_equal(const char *s, char c)
{
	int	i;

	i = 0;
	if (c == 0)
		return (i + ft_strlen(s));
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (0);
}

char	*ft_strndup_value(const char *s1, int n)
{
	char	*arr;
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(s1) - n;
	arr = (char *)malloc((len + 1) * sizeof(char));
	if (!arr)
		return (0);
	while (s1[n])
		arr[i++] = s1[n++];
	arr[i] = 0;
	return (arr);
}

char	*make_env_name(char *str)
{
	if(!(find_equal(str, '=')))
		return(ft_strdup(str));
	else
		return(ft_strndup(str, find_equal(str, '=')));
}

char	*make_env_value(char *str)
{
	if(!(find_equal(str, '=')))
		return(ft_strdup(""));
	else
		return(ft_strndup_value(str, find_equal(str, '=') + 1));
}

t_envval	*make_env(char *str)
{
	t_envval	*env;

	env = ft_calloc(1, sizeof(t_envval));
	if(!env)
		return (T_NULL);
	env->name = make_env_name(str);
	env->value = make_env_value(str);
	if (env->name == T_NULL || env->value == T_NULL)
	{
		envval_delete_func((void *)env);
		return (T_NULL);
	}
	return(env);
}
//export구동부---------------------------------------------------------

int	check_start_char_export(char *str)
{
	if (!(str[0] >= 'a' && str[0] <= 'z') && !(str[0] >= 'A' && str[0] <= 'Z'))
	{
		printf("bash: export: %s: not a valid identifier\n", str);
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
	if (dll)
	{
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
				if (env->value[0] != '\0')
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
	}
	return (flag);
}

int	ft_export(t_data *data, char **str)
{
	int	i;
	int	flag;
	int	result;
	t_dll *dll;
	t_envval *tmp;

	if (str[1] == T_NULL)
	{
		dll_print(&data->sorted_envdll, dll_export_print_func);
		return (0);
	}
	dll = &data->envdll;
	flag = 0;
	i = 1;
	while (str[i])
	{
		//printf ("NAME : %s	VALUE : %s\n", make_env(str[i])->name, make_env(str[i])->value);
		tmp = make_env(str[i]);
		if (tmp == T_NULL)
			return (-1);
		result = do_export(dll, &data->sorted_envdll, str[i], tmp);
		if (result == -1)
			return (-1);
		flag += result;
		i++;
	}
	if (flag)
		return(1);
	return (0);
}