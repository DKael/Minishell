/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:10:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 00:53:55 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void data_init(t_data *data, char *program_name, char **envp)
{
	int idx;

	data->program_name = program_name;
	data->envp = envp;
	set_last_exit_code(data, 0);
	data->old_stdin = 253;
	data->old_stdout = 254;
	data->old_stderr = 255;
	idx = -1;
	while (++idx < 256)
		data->opened_fd[idx] = -1;
	data->wd[0] = '\0';
	data->wd[MAX_PATH_LEN - 1] = '\0';
	if (getcwd(data->wd, MAX_PATH_LEN) == T_NULL)
	{
		write(2, "shell-init: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 116);
	}
	dll_init(&data->envdll);
	dll_init(&data->sorted_envdll);
	store_env_in_dll(data, envp);
}

void store_env_in_dll(t_data *data, char **envp)
{
	t_envval *env;
	int idx;

	while (*envp)
	{
		env = (t_envval *)ft_calloc(1, sizeof(t_envval));
		if (env == T_NULL)
		{
			dll_clear(&data->envdll, envval_delete_func);
			message_exit("minishell: malloc error10!\n", 1);
		}
		idx = -1;
		while ((*envp)[++idx] != '=' && (*envp)[idx] != '\0')
			;
		if ((*envp)[idx] == '=')
		{
			env->name = ft_strndup(*envp, idx);
			if (ft_strncmp((*envp), "SHLVL", idx) == 0)
			{
				if (ft_isdecimal(&(*envp)[idx + 1]) == TRUE)
					env->value = ft_itoa(ft_atoi(&(*envp)[idx + 1]) + 1);
				else
					env->value = ft_itoa(1);
			}
			else if (ft_strncmp((*envp), "OLDPWD", idx) == 0)
				;
			else
				env->value = ft_strdup(&(*envp)[idx + 1]);
		}
		else
			env->name = ft_strdup(*envp);
		if (env->name == T_NULL || ((*envp)[idx] == '=' && ft_strncmp((*envp), "OLDPWD", idx) != 0 && env->value == T_NULL))
		{
			dll_clear(&data->envdll, envval_delete_func);
			envval_delete_func((void *)env);
			message_exit("minishell: malloc error11!\n", 1);
		}
		if (dll_content_add(&data->envdll, (void *)env, 0) == FALSE)
		{
			dll_clear(&data->envdll, envval_delete_func);
			envval_delete_func((void *)env);
			message_exit("minishell: malloc error12!\n", 1);
		}
		envp++;
	}
	envdll_sorting(data);
}

void envdll_sorting(t_data *data)
{
	t_srt *tmp;
	t_dllnode *node_ptr;
	int idx;
	

	tmp = (t_srt *)ft_calloc(data->envdll.size, (sizeof(t_srt)));
	if (tmp == T_NULL)
	{
		dll_clear(&data->envdll, envval_delete_func);
		message_exit("minishell: malloc error13!\n", 1);
	}
	idx = -1;
	node_ptr = data->envdll.head.back;
	while (++idx < data->envdll.size)
	{
		tmp[idx].name = ((t_envval *)(node_ptr->contents))->name;
		tmp[idx].ptr = node_ptr;
		node_ptr = node_ptr->back;
	}
	sorting((void *)tmp, data->envdll.size, srt_compare, srt_swap);
	idx = -1;
	while (++idx < data->envdll.size)
	{
		if (dll_content_add(&data->sorted_envdll, tmp[idx].ptr, 0) == FALSE)
		{
			ft_free1((void **)&tmp);
			dll_clear(&data->envdll, envval_delete_func);
			dll_clear(&data->sorted_envdll, T_NULL);
			message_exit("minishell: malloc error14!\n", 1);
		}
	}
	ft_free1((void **)&tmp);
	check_oldpwd_exist(data);
}

void	check_oldpwd_exist(t_data *data)
{
	char	*oldpwd[3];

	oldpwd[0] = "export";
	oldpwd[1] = "OLDPWD";
	oldpwd[2] = T_NULL;
	if (ft_export(&data->envdll, &data->sorted_envdll, oldpwd) == -1)
	{
		dll_clear(&data->envdll, envval_delete_func);
		dll_clear(&data->sorted_envdll, T_NULL);
		message_exit("minishell: malloc error15!\n", 1);
	}
}

void data_cycle_init(t_data *data)
{
	int idx;

	data->cmd = T_NULL;
	data->tkn = T_NULL;
	data->ao_cnt = 0;
	data->pipe_cnt = T_NULL;
	data->pp = T_NULL;
	data->pid_table = T_NULL;
	data->logic_table = T_NULL;
	idx = -1;
	while (data->opened_fd[++idx] != -1)
		data->opened_fd[idx] = -1;
}
