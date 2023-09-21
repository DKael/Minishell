#include "minishell.h"

t_bool change_pwd_oldpwd(t_dll *env, char *path)
{
	t_dllnode *ptr;
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
	}
	else
	{
		if (tmp_env[1] != T_NULL)
		{
			ft_free1((void **)&tmp_env[1]->value);
			tmp_env[1]->value = ft_strdup(path);
			if (tmp_env[1]->value == T_NULL)
				return (FALSE);
		}
	}
	return (TRUE);
}

int	check_path(char *path)
{

}

int	ft_cd(t_dll *dll, char **input)
{
	int	idx;
	t_dllnode *ptr;
	t_envval	*tmp_env;

	idx = -1;
	while (input[++idx] != T_NULL)
		;
	if (idx >= 3)
		err_msg_print3("cd: ", input[2], ": No such file or directory");
	else if (idx == 1)
	{
		ptr = dll->head.back;
		while (ptr != &(data->envdll.tail))
		{
			tmp_env = (t_envval *)(ptr->contents);
			if (ft_strcmp(tmp_env->name, "HOME") == 0)
			{
				if (tmp_env->value[0] == '\0')
				{
					err_msg_print1("cd: HOME not set");
					return (1);
				}
				if (change_pwd_oldpwd(&data->envdll, (char *)tmp_env->value) == FALSE
					|| chdir((char *)tmp_env->value) == -1)
					return (-1);
				break;
			}
			ptr = ptr->back;
		}
		if (ptr == &(data->envdll.tail))
		{
			err_msg_print1("cd: HOME not set");
			return (1);
		}
	}
	else
	{

	}
	return (0);
}