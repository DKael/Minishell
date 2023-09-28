/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 00:19:39 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 21:33:12 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_bool	change_pwd_oldpwd(t_dll *env, char *path);
static int		cd_mode1_mode2(t_data *data, int mode);
static int		cd_case3(t_data *data, char *input);
static int		cd_case3_2(t_data *data, char *path);
static int		do_cd(t_data *data, t_dll *env, char *path);

int	ft_cd(t_data *data, char **input)
{
	int		idx;

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
			return (cd_mode1_mode2(data, 1));
		else
			return (cd_mode1_mode2(data, 2));
	}
	else
		return (cd_case3(data, input[1]));
}

static int	cd_mode1_mode2(t_data *data, int mode)
{
	char	*env_name;
	char	*path;
	int		result;

	env_name = "HOME";
	if (mode == 2)
		env_name = "OLDPWD";
	path = ft_getenv(data, env_name);
	if (path == T_NULL)
	{
		err_msg_print3("cd: ", env_name, " not set");
		return (1);
	}
	result = check_file(path);
	if (result != 0)
		return (result);
	path = remove_duplicate_slashs(path);
	if (path == T_NULL)
		return (-1);
	result = do_cd(data, &data->envdll, path);
	if (result != 0)
		return (result);
	if (mode == 2)
		printf("%s\n", path);
	return (ft_free3((void **)&path, 0));
}

static	int	cd_case3(t_data *data, char *input)
{
	int		result;
	char	*path[2];

	result = check_file(input);
	if (result != 0)
		return (result);
	path[0] = remove_duplicate_slashs(input);
	if (path[0] == T_NULL)
		return (-1);
	if (path[0][0] != '/')
	{
		path[1] = make_path(path[0], 0);
		ft_free1((void **)&path[0]);
		if (path[1] == T_NULL)
		{
			err_msg_print1("cd: malloc error1");
			return (-1);
		}
		else if (path[1] == (char *)-1 || path[1] == (char *)-2)
			return (0);
	}
	else
		path[1] = path[0];
	return (cd_case3_2(data, path[1]));
}

static	int	cd_case3_2(t_data *data, char *path)
{
	int	result;

	result = do_cd(data, &data->envdll, path);
	if (result != 0)
		return (result);
	return (ft_free3((void **)&path, 0));
}

static	int	do_cd(t_data *data, t_dll *env, char *path)
{
	if (ft_chdir(path, data) == -1)
		return (ft_free3((void **)&path, -2));
	if (change_pwd_oldpwd(env, path) == FALSE)
		return (ft_free3((void **)&path, -1));
	return (0);
}
