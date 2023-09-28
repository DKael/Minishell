/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd1_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:50:55 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:59:19 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int		count_ao_in_cmd(char *cmd);
static char		**split_cmd_by_ao(char *cmd, int ao_cnt, t_logic *logic_table);
static t_bool	split_cmd_by_ao2(char *cmd, char **tmp,
					t_logic *logic_table, int *idx);
extern t_dll	**tokenize(char *tkns, int *pipe_cnt);

void	cmd_split_error(t_data *data, char *cmd, char *msg)
{
	int	idx1;

	err_msg_print1(msg);
	ft_free1((void **)&cmd);
	dll_clear(&data->envdll, envval_delete_func);
	free_2d_array((void ***)&data->ao_split, data->ao_cnt);
	idx1 = -1;
	while (++idx1 < data->ao_cnt)
		free_2d_dll(&data->tkn[idx1], data->pipe_cnt[idx1], str_delete_func);
	ft_free1((void **)&data->tkn);
	ft_free1((void **)&data->pipe_cnt);
	ft_free1((void **)&data->logic_table);
	exit(1);
}

void	split_cmd(t_data *data, char *cmd)
{
	int	idx;

	data->ao_cnt = count_ao_in_cmd(cmd) + 1;
	data->logic_table = (t_logic *)ft_calloc(data->ao_cnt - 1, sizeof(t_logic));
	if (data->logic_table == T_NULL)
	{
		ft_free1((void **)&cmd);
		exit(1);
	}
	data->ao_split = split_cmd_by_ao(cmd, data->ao_cnt, data->logic_table);
	data->tkn = (t_dll ***)ft_calloc(data->ao_cnt, sizeof(t_dll **));
	data->pipe_cnt = (int *)ft_calloc(data->ao_cnt, sizeof(int));
	if (data->ao_split == T_NULL || data->tkn == T_NULL
		|| data->pipe_cnt == T_NULL)
		cmd_split_error(data, cmd, "malloc error");
	idx = -1;
	while (++idx < data->ao_cnt)
	{
		data->tkn[idx] = tokenize(data->ao_split[idx], &data->pipe_cnt[idx]);
		if (data->tkn[idx] == T_NULL)
			cmd_split_error(data, cmd, "malloc error");
	}
}

static int	count_ao_in_cmd(char *cmd)
{
	int	idx;
	int	ao_cnt;

	idx = -1;
	ao_cnt = 0;
	while (cmd[++idx] != '\0')
	{
		if ((cmd[idx] == '|' && cmd[idx + 1] == '|')
			|| (cmd[idx] == '&' && cmd[idx + 1] == '&'))
		{
			ao_cnt++;
			idx++;
		}
		else if (cmd[idx] == '\"' || cmd[idx] == '\'')
			ignore_quote(cmd, &idx);
		else if (cmd[idx] == '(')
			ignore_parentheses(cmd, &idx);
	}
	return (ao_cnt);
}

static char	**split_cmd_by_ao(char *cmd, int ao_cnt, t_logic *logic_table)
{
	char	**tmp;
	int		idx[3];

	tmp = (char **)ft_calloc(ao_cnt, sizeof(char *));
	if (tmp == T_NULL)
		return (T_NULL);
	idx[2] = 0;
	idx[1] = 0;
	idx[0] = -1;
	while (cmd[++idx[0]] != '\0')
		if (split_cmd_by_ao2(cmd, tmp, logic_table, idx) == FALSE)
			return (T_NULL);
	tmp[idx[1]] = ft_strndup(&cmd[idx[2]], idx[0] - idx[2]);
	if (tmp[idx[1]] == T_NULL)
		return (free_2d_array((void ***)&tmp, idx[1]));
	return (tmp);
}

static t_bool	split_cmd_by_ao2(char *cmd, char **tmp,
	t_logic *logic_table, int *idx)
{
	if ((cmd[idx[0]] == '|' && cmd[idx[0] + 1] == '|')
		|| (cmd[idx[0]] == '&' && cmd[idx[0] + 1] == '&'))
	{
		if (cmd[idx[0]] == '|' && cmd[idx[0] + 1] == '|')
			logic_table[idx[1]] = OR;
		else if (cmd[idx[0]] == '&' && cmd[idx[0] + 1] == '&')
			logic_table[idx[1]] = AND;
		tmp[idx[1]] = ft_strndup(&cmd[idx[2]], idx[0] - idx[2]);
		if (tmp[idx[1]] == T_NULL)
		{
			free_2d_array((void ***)&tmp, idx[1]);
			return (FALSE);
		}
		idx[2] = idx[0] + 2;
		idx[1]++;
		idx[0]++;
	}
	else if (cmd[idx[0]] == '\"' || cmd[idx[0]] == '\'')
		ignore_quote(cmd, &idx[0]);
	else if (cmd[idx[0]] == '(')
		ignore_parentheses(cmd, &idx[0]);
	return (TRUE);
}
