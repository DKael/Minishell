#include "minishell.h"

int	check_start_char_unset(char *str)
{
	if (!(str[0] >= 'a' && str[0] <= 'z') && !(str[0] >= 'A' && str[0] <= 'Z'))
	{
		printf("bash: unset: '%s': not a valid identifier\n", str);
		return (1);
	}
	return(0);
}

int	do_unset(t_dll *sorted_env, t_dll *env, char *str)
{
	int	flag;
	t_dllnode	*dllnode;
	char	*tmp;

	flag = 0;
	if (sorted_env)
	{
		flag += check_start_char_unset(str);
		dllnode = sorted_env->head.back;
		while (dllnode != &(sorted_env->tail))
		{
			tmp = (char *)((t_envval *)((((t_dllnode *)(dllnode->contents))->contents)))->name;
			if (!(ft_strcmp(tmp, str)))
			{
				dll_del_node(env, dllnode->contents, envval_delete_func);
				dll_del_node(sorted_env, dllnode, T_NULL);
				break;
			}
			dllnode = dllnode->back;
		}
	}
	return (flag);
}

int	ft_unset(t_data *data, char **str)
{
	int	i;
	int	flag;
	t_dll	*sorted_env;
	
	sorted_env = &data->sorted_envdll;
	flag = 0;
	i = 1;
	while (str[i])
	{
		flag += do_unset(sorted_env, &data->envdll, str[i]);
		i++;
	}
	if (flag)
		return(1);
	return (0);
}