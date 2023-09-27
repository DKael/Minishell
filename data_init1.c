/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:10:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 17:47:34 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_envval	*make_envval(t_data *data, char *envp, int idx);
static void		make_envdll(t_data *data, char **envp);
static void		make_sorted_envdll(t_data *data);
static void		make_sorted_envdll2(t_data *data, t_srt *tmp);
extern void		check_oldpwd_exist(t_data *data);

void	data_cycle_init(t_data *data)
{
	dont_make_command_str();
	signal(SIGINT, signal_handler);
	data->cmd = T_NULL;
	data->tkn = T_NULL;
	data->ao_split = T_NULL;
	data->ao_cnt = 0;
	data->pipe_cnt = T_NULL;
	data->pp = T_NULL;
	data->pid_table = T_NULL;
	data->logic_table = T_NULL;
}

void	data_init(t_data *data, char *program_name, char **envp)
{
	int	idx;

	dont_make_command_str();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	data->program_name = program_name;
	data->envp = envp;
	data->w_status = 0;
	set_exit_code(data, 0);
	data->old_stdin = 253;
	data->old_stdout = 254;
	data->old_stderr = 255;
	if (basic_redirection_save(data) == FALSE)
		exit(1);
	idx = -1;
	while (++idx < MAX_FD)
		data->opened_fd[idx] = -1;
	data->wd[0] = '\0';
	data->wd[MAX_PATH_LEN - 1] = '\0';
	if (getcwd(data->wd, MAX_PATH_LEN) == T_NULL)
		message_exit("shell-init: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 1);
	dll_init(&data->envdll);
	dll_init(&data->sorted_envdll);
	make_envdll(data, envp);
}

static void	make_envdll(t_data *data, char **envp)
{
	t_envval	*env;
	int			idx;

	while (*envp)
	{
		idx = -1;
		while ((*envp)[++idx] != '=' && (*envp)[idx] != '\0')
			;
		env = make_envval(data, *envp, idx);
		if (env == T_NULL)
		{
			envp++;
			continue ;
		}
		if (((ft_strncmp((*envp), "OLDPWD", idx) != 0 && env->value == T_NULL)
				|| env->name == T_NULL)
			|| dll_content_add(&data->envdll, (void *)env, 0) == FALSE)
		{
			dll_clear(&data->envdll, envval_delete_func);
			envval_delete_func((void *)env);
			message_exit("minishell: malloc error\n", 1);
		}
		envp++;
	}
	make_sorted_envdll(data);
}

static void	make_sorted_envdll(t_data *data)
{
	t_srt		*tmp;
	t_dllnode	*node_ptr;
	int			idx;

	tmp = (t_srt *)ft_calloc(data->envdll.size, (sizeof(t_srt)));
	if (tmp == T_NULL)
	{
		dll_clear(&data->envdll, envval_delete_func);
		message_exit("minishell: malloc error\n", 1);
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
	make_sorted_envdll2(data, tmp);
}

static void	make_sorted_envdll2(t_data *data, t_srt *tmp)
{
	int	idx;

	idx = -1;
	while (++idx < data->envdll.size)
	{
		if (dll_content_add(&data->sorted_envdll, tmp[idx].ptr, 0) == FALSE)
		{
			ft_free1((void **)&tmp);
			dll_clear(&data->envdll, envval_delete_func);
			dll_clear(&data->sorted_envdll, T_NULL);
			message_exit("minishell: malloc error\n", 1);
		}
	}
	ft_free1((void **)&tmp);
	check_oldpwd_exist(data);
}
