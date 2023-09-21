/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 16:28:59 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/21 17:42:37 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **make_2d_array_from_dll(t_dll *dll);
char **make_2d_envp_from_dll(t_dll *dll);
char *get_cmd_path(char **path, char *cmd);
static char *get_cmd_path2(char **path, char *cmd);

void child(t_data *data, int ao_idx, int pp_idx)
{
	t_dll *tmp;
	t_cmd_info *cmd_info;
	t_dllnode *node_ptr[2];
	int result;
	char *raw_path;
	char **split_path;
	char *cmd_path;
	char *cmd;
	char **argu_lst;
	char **envp_lst;
	int idx;


	tmp = data->tkn[ao_idx][pp_idx];
	cmd_info = (t_cmd_info *)(tmp->head.contents);
	if (data->pipe_cnt[ao_idx] > 1)
		if (pipe_redirection(data, ao_idx, pp_idx) == FALSE)
			resource_free_and_exit(data, 1, "redirection error");
	result = sign_redirection(data, tmp);
	if (result == 1)
		exit(1);
	else if (result == 2)
		resource_free_and_exit(data, 1, "file open error");
	else if (result == 3)
		resource_free_and_exit(data, 1, "redirection error");
	else if (result == 4)
		resource_free_and_exit(data, 1, "stat() error");
	node_ptr[0] = tmp->head.back;
	if (cmd_info->parentheses_flag == TRUE)
	{
		// have to make
	}
	else
	{
		cmd = (char *)(node_ptr[0]->contents);
		idx = is_builtin_func(cmd);
		if (idx == 0)
		{
			raw_path = ft_getenv(data, "PATH");
			if (raw_path != T_NULL)
			{
				split_path = ft_split(raw_path, ':');
				if (split_path == T_NULL)
					resource_free_and_exit(data, 1, "malloc error");
			}
			else
				split_path = T_NULL;
			cmd_path = get_cmd_path(split_path, cmd);
			free_2d_array2((void ***)&split_path);
			if (cmd_path == (char *)-1)
				resource_free_and_exit(data, 1, "malloc error");
			fprintf(stderr, "%d_%d cmd's path is : %s\n", ao_idx, pp_idx, cmd_path);
			if (cmd_path == T_NULL && split_path != T_NULL)
			{
				err_msg_print2(cmd, ": command not found");
				exit(127);
			}
			else if (cmd_path == T_NULL && split_path == T_NULL)
			{
				write(2, "minishell: ", 11);
				perror(cmd);
				exit(127);
			}
			argu_lst = make_2d_array_from_dll(tmp);
			envp_lst = make_2d_envp_from_dll(&data->envdll);
			if (argu_lst == T_NULL || envp_lst == T_NULL)
			{
				free(argu_lst);
				free(envp_lst);
				resource_free_and_exit(data, 1, "malloc error");
			}
			if (execve(cmd_path, argu_lst, envp_lst) == -1)
			{
				free(argu_lst);
				free(envp_lst);
				resource_free_and_exit(data, 1, "malloc error");
			}
		}
		else
		{
			argu_lst = make_2d_array_from_dll(data->tkn[ao_idx][0]);
			if (argu_lst == T_NULL)
				resource_free_and_exit(data, 1, "malloc error");
			result = execute_builtin_func(idx, argu_lst, data);
			if (result == -1)
			{
				free(argu_lst);
				resource_free_and_exit(data, 1, "malloc error");
			}
			exit (result);
		}
	}
}

char **make_2d_array_from_dll(t_dll *dll)
{
	char **tmp;
	int idx;
	t_dllnode *node_ptr;

	tmp = (char **)ft_calloc(dll->size + 1, sizeof(char *));
	if (tmp == T_NULL)
		return (T_NULL);
	node_ptr = dll->head.back;
	idx = -1;
	while (node_ptr != &(dll->tail))
{
	tmp[++idx] = (char *)node_ptr->contents;
	node_ptr = node_ptr->back;
}
		
	return (tmp);
}

char **make_2d_envp_from_dll(t_dll *dll)
{
	char **tmp;
	char *tmp2;
	int idx;
	t_dllnode *node_ptr;
	t_envval *ptr;

	tmp = (char **)ft_calloc(dll->size + 1, sizeof(char *));
	if (tmp == T_NULL)
		return (T_NULL);
	node_ptr = dll->head.back;
	idx = -1;
	while (node_ptr != &(dll->tail))
	{
		ptr = (t_envval *)node_ptr->contents;
		tmp2 = ft_strjoin2(ptr->name, ptr->value, "=");
		if (tmp2 == T_NULL)
			return (free_2d_array2((void ***)&tmp));
		tmp[++idx] = tmp2;
		node_ptr = node_ptr->back;
	}
	return (tmp);
}

char *get_cmd_path(char **path, char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (cmd);
	if (path == T_NULL)
		return (T_NULL);
	return (get_cmd_path2(path, cmd));
}

static char *get_cmd_path2(char **path, char *cmd)
{
	int idx;
	char *slash_cmd;
	char *tmp_cmd;

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