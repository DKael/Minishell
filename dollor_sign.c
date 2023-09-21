/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollor_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:58:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/22 02:07:39 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	dll_dollor_tmp_print_func(void *content)
// {
// 	t_dollor_tmp *tmp;

// 	tmp = (t_dollor_tmp *)content;
// 	printf("%s\n", tmp->value);
// }

t_bool	retrieve_variable_value(t_data *data, t_dll *tkns)
{
	t_dll temp_dll;
	t_dllnode *node_ptr;
	t_dllnode *node_ptr2;
	t_dollor_tmp *dtmp;
	char	*tkn;
	int		idx;
	int		idx2;
	int		write_idx;
	char	*tmp;
	int		expansion_len;
	int		idx_chk;

	if (tkns == T_NULL)
		return (FALSE);
	dll_init(&temp_dll);
	node_ptr = tkns->head.back;
	while (node_ptr != &(tkns->tail))
	{
		expansion_len = 0;
		tkn = (char *)node_ptr->contents;
		idx = -1;
		while (tkn[++idx] != '\0')
		{
			if (tkn[idx] == -1)
			{
				dtmp = (t_dollor_tmp *)ft_calloc(1, sizeof(t_dollor_tmp));
				if (dtmp == T_NULL)
				{
					dll_clear(&temp_dll, T_NULL);
					return (FALSE);
				}
				idx_chk = idx;
				tmp = get_dollor_parameter(tkn, &idx);
				if (tmp == T_NULL)
				{
					dll_clear(&temp_dll, T_NULL);
					return (ft_free2((void **)dtmp, FALSE));
				}
				dtmp->value = ft_getenv(data, tmp);
				ft_free1((void **)&tmp);
				if (dtmp->value == T_NULL)
					dtmp->value = "";
				dtmp->idx_jump = idx - idx_chk;
				if (dll_content_add(&temp_dll, dtmp, 0) == FALSE)
				{
					dll_clear(&temp_dll, T_NULL);
					return (ft_free2((void **)dtmp, FALSE));
				}
				expansion_len += (ft_strlen(dtmp->value) + (idx - idx_chk + 1));
			}
		}
		if (temp_dll.size == 0)
		{
			node_ptr = node_ptr->back;
			continue;
		}
		tmp = (char *)ft_calloc(ft_strlen(tkn) + expansion_len + 1, sizeof(char));
		if (tmp == T_NULL)
		{
			dll_clear(&temp_dll, T_NULL);
			return (FALSE);
		}
		idx = -1;
		write_idx = -1;
		node_ptr2 = temp_dll.head.back;
		while (tkn[++idx] != '\0')
		{
			if (tkn[idx] == -1)
			{
				dtmp = (t_dollor_tmp *)(node_ptr2->contents);
				idx2 = -1;
				while ((dtmp->value)[++idx2] != '\0')
					tmp[++write_idx] = (dtmp->value)[idx2];
				idx += dtmp->idx_jump;
				node_ptr2 = node_ptr2->back;
			}
			else
				tmp[++write_idx] = tkn[idx];
		}
		free(node_ptr->contents);
		node_ptr->contents = tmp;
		dll_clear(&temp_dll, T_NULL);
		node_ptr = node_ptr->back;
	}
	return (TRUE);
}

char *ft_getenv(t_data *data, const char *name)
{
	t_dllnode *ptr;

	if (name[0] == '?' && name[1] == '\0')
		return (data->last_exit_code_str);
	ptr = data->envdll.head.back;
	while (ptr != &(data->envdll.tail))
	{
		if (ft_strcmp(((t_envval *)(ptr->contents))->name, name) == 0)
			return (((t_envval *)(ptr->contents))->value);
		ptr = ptr->back;
	}
	return (T_NULL);
}

char *get_dollor_parameter(char *cmd, int *idx)
{
	char *result;
	int idx_chk;

	if (cmd[(*idx) + 1] == '{')
	{
		idx_chk = (*idx) + 2;
		(*idx) += 1;
		while (cmd[++(*idx)] != '}')
			;
		result = (char *)ft_strndup(&cmd[idx_chk], (*idx) - idx_chk);
	}
	else if (cmd[(*idx) + 1] == '?')
	{
		(*idx)++;
		return ((char *)ft_strdup("?"));
	}
	else
	{
		idx_chk = (*idx) + 1;
		while (cmd[++(*idx)] != '\0' && cmd[(*idx)] != '{' && cmd[(*idx)] != '\"'
		&& cmd[(*idx)] != '\'' && ft_isblank(cmd[(*idx)]) == FALSE && cmd[(*idx)] != '$')
			;
		result = (char *)ft_strndup(&cmd[idx_chk], (*idx) - idx_chk);
		(*idx)--;
	}
	return (result);
}