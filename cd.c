/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 00:19:39 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 01:09:26 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_chdir(char *path, t_data *data)
{
	if (chdir(path) == -1)
		return (-1);
	ft_strcpy(data->wd, path);
	return (0);
}

t_bool change_pwd_oldpwd(t_dll *env, char *path)
{
	t_dllnode *ptr;
	t_envval *tmp_env[3];

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

char *make_path(char *raw_path, int mode)
{
	char	buffer[MAX_PATH_LEN];
	char	**split;
	int idx;
	int	idx2;
	int bf_idx;

	if (getcwd(buffer, MAX_PATH_LEN) == T_NULL)
	{
		if (mode == 0)
			write(2, "cd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 108);
		return ((char *)-1);
	}
	bf_idx = -1;
	while (buffer[++bf_idx] != '\0')
		;
	split = ft_split(raw_path, '/');
	if (split == T_NULL)
		return (T_NULL);
	idx = -1;
	while (split[++idx] != T_NULL)
	{
		if (split[idx][0] == '.' && split[idx][1] == '.' && split[idx][2] == '\0')
		{
			while (buffer[--bf_idx] != '/')
				;
			if (bf_idx == 0)
				buffer[++bf_idx] = '\0';
			else
				buffer[bf_idx] = '\0';
		}
		else if ((split[idx][0] == '.' && split[idx][1] == '\0') || split[idx][0] == '\0')
			continue;
		else
		{
			if (bf_idx + 2 + ft_strlen(split[idx]) > MAX_PATH_LEN)
			{
				err_msg_print3("cd: ", split[idx], ": File name too long");
				free_2d_array2((void ***)&split);
				return ((char *)-1);
			}
			buffer[bf_idx] = '/';
			idx2 = -1;
			while (split[idx][++idx2] != '\0')
				buffer[++bf_idx] = split[idx][idx2];
			buffer[++bf_idx] = '\0';
		}
	}
	free_2d_array2((void ***)&split);
	return (ft_strdup(buffer));
}

char *remove_duplicate_slashs(char *str)
{
	char *tmp;
	int idx1;
	int idx2;

	tmp = ft_strdup(str);
	if (tmp == T_NULL)
		return (T_NULL);
	idx1 = -1;
	idx2 = -1;
	while (str[++idx1] != '\0')
	{
		if (str[idx1] == '/')
		{
			tmp[++idx2] = str[idx1];
			while (str[++idx1] == '/')
				;
			idx1--;
		}
		else
			tmp[++idx2] = str[idx1];
	}
	tmp[++idx2] = '\0';
	return (tmp);
}

int ft_cd(t_data *data, t_dll *dll, char **input)
{
	int idx;
	char *raw_path;
	char *path;
	char *env_name;
	int result;
	t_file_info info;

	idx = -1;
	while (input[++idx] != T_NULL)
		;
	if (idx >= 3)
	{
		err_msg_print3("cd: ", input[2], ": Too many arguments");
		return (1);
	}
	else if (idx == 2 && input[1][0] == '\0')
		return (0);
	if (idx == 1 || (input[1][0] == '-' && input[1][1] == '\0'))
	{
		if (idx == 1)
			env_name = "HOME";
		else
			env_name = "OLDPWD";
		raw_path = ft_getenv(data, env_name);
		if (raw_path == T_NULL)
		{
			err_msg_print3("cd: ", env_name, " not set");
			return (1);
		}
		result = get_file_info(raw_path, &info, 1);
		if (result != 0)
		{
			if (result == 1)
				err_msg_print3("cd: ", raw_path, ": No such file or directory");
			return (result);
		}
		if (info.type == REGULAR_FILE)
		{
			err_msg_print3("cd: ", raw_path, ": Not a directory");
			return (1);
		}
		if ((info.mode & 0700) != 0700)
		{
			err_msg_print3("cd: ", raw_path, ": Permission denied");
			return (1);
		}
		path = remove_duplicate_slashs(raw_path);
		if (path == T_NULL)
			return (-1);
		if (ft_chdir(path, data) == -1)
		{
			ft_free1((void **)&path);
			return (-2);
		}
		if (change_pwd_oldpwd(dll, path) == FALSE)
		{
			ft_free1((void **)&path);
			return (-1);
		}
		if (idx == 2 && input[1][0] == '-' && input[1][1] == '\0')
			printf("%s\n", path);
		ft_free1((void **)&path);
	}
	else
	{
		result = get_file_info(input[1], &info, 1);
		if (result != 0)
		{
			if (result == 1)
				err_msg_print3("cd: ", input[1], ": No such file or directory");
			return (result);
		}
		if (info.type == REGULAR_FILE)
		{
			err_msg_print3("cd: ", input[1], ": Not a directory");
			return (1);
		}
		if ((info.mode & 0700) != 0700)
		{
			err_msg_print3("cd: ", input[1], ": Permission denied");
			return (1);
		}
		raw_path = remove_duplicate_slashs(input[1]);
		if (raw_path == T_NULL)
			return (-1);
		if (raw_path[0] != '/')
		{
			path = make_path(raw_path, 0);
			ft_free1((void **)&raw_path);
			if (path == T_NULL)
			{
				err_msg_print1("cd: malloc error1");
				return (-1);
			}
			else if (path == (char *)-1)
				return (0);
		}
		else
			path = raw_path;
		if (ft_chdir(path, data) == -1)
		{
			ft_free1((void **)&path);
			return (-2);
		}
		if (change_pwd_oldpwd(dll, path) == FALSE)
		{
			ft_free1((void **)&path);
			return (-1);
		}
		ft_free1((void **)&path);
	}
	return (0);
}