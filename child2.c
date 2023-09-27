/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 23:32:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 23:38:00 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cmd_path2(char **path, char *cmd);

void	child_execute_builtin(t_data *data, t_cdata *cdata, int func_type)
{
	int	result;

	cdata->argu_lst = make_2d_array_from_dll(cdata->tkns);
	if (cdata->argu_lst == T_NULL)
		child_exit(data, cdata->pipe_cnt, 1, "malloc error");
	result = execute_builtin_func(func_type, cdata->argu_lst, data);
	child_exit(data, cdata->pipe_cnt, result, T_NULL);
}

char	*get_cmd_path(char **path, char *cmd)
{
	char	*result;

	if (access(cmd, X_OK) == 0)
	{
		result = ft_strdup(cmd);
		if (result == T_NULL)
			return ((char *)-1);
		return (result);
	}
	if (path == T_NULL)
		return (T_NULL);
	return (get_cmd_path2(path, cmd));
}

static char	*get_cmd_path2(char **path, char *cmd)
{
	int		idx;
	char	*slash_cmd;
	char	*tmp_cmd;

	slash_cmd = ft_strjoin("/", cmd);
	if (slash_cmd == T_NULL)
		return ((char *)-1);
	idx = -1;
	while (path[++idx])
	{
		tmp_cmd = ft_strjoin(path[idx], slash_cmd);
		if (tmp_cmd == T_NULL)
			return ((char *)-1);
		if (access(tmp_cmd, X_OK) == 0)
		{
			free(slash_cmd);
			return (tmp_cmd);
		}
		free(tmp_cmd);
	}
	free(slash_cmd);
	return (T_NULL);
}
