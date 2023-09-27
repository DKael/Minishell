/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollor_sign1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:58:25 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 14:00:01 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	retrieve_variable_value2(t_dll *tmp_dll,
					int e_len, char *tkn, t_dllnode *node_ptr);
static t_bool	find_minus_one(t_data *data, t_dll *tmp_dll,
					char *tkn, int *e_len);
static t_bool	find_minus_one2(t_data *data, char *tkn,
					int *idx, t_dollor_tmp *dtmp);
static char		*do_expansion(t_dll *tmp_dll, char *tkn, int e_len);
extern char		*get_dollor_parameter(char *cmd, int *idx);

t_bool	retrieve_variable_value(t_data *data, t_dll *tkns)
{
	t_dll			tmp_dll;
	t_dllnode		*node_ptr;
	int				e_len;

	dll_init(&tmp_dll);
	node_ptr = tkns->head.back;
	while (node_ptr != &(tkns->tail))
	{
		e_len = 0;
		if (find_minus_one(data, &tmp_dll, node_ptr->contents, &e_len) == FALSE)
		{
			dll_clear(&tmp_dll, T_NULL);
			return (FALSE);
		}
		if (tmp_dll.size == 0)
		{
			node_ptr = node_ptr->back;
			continue ;
		}
		if (retrieve_variable_value2(&tmp_dll, e_len,
				(char *)node_ptr->contents, node_ptr) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

static t_bool	retrieve_variable_value2(t_dll *tmp_dll, int e_len,
	char *tkn, t_dllnode *node_ptr)
{
	tkn = do_expansion(tmp_dll, tkn, e_len);
	if (tkn == T_NULL)
	{
		dll_clear(tmp_dll, T_NULL);
		return (FALSE);
	}
	free(node_ptr->contents);
	node_ptr->contents = tkn;
	dll_clear(tmp_dll, T_NULL);
	node_ptr = node_ptr->back;
	return (TRUE);
}

static t_bool	find_minus_one(t_data *data, t_dll *tmp_dll, char *tkn,
	int *e_len)
{
	t_dollor_tmp	*dtmp;
	int				idx[2];

	idx[0] = -1;
	while (tkn[++idx[0]] != '\0')
	{
		if (tkn[idx[0]] == -1)
		{
			dtmp = (t_dollor_tmp *)ft_calloc(1, sizeof(t_dollor_tmp));
			if (dtmp == T_NULL)
				return (FALSE);
			idx[1] = idx[0];
			if (find_minus_one2(data, tkn, idx, dtmp) == FALSE)
				return (FALSE);
			if (dll_content_add(tmp_dll, dtmp, 0) == FALSE)
				return (ft_free2((void **)&dtmp, FALSE));
			(*e_len) += (ft_strlen(dtmp->value) - (idx[0] - idx[1] + 1));
		}
	}
	return (TRUE);
}

static t_bool	find_minus_one2(t_data *data, char *tkn, int *idx,
	t_dollor_tmp *dtmp)
{
	char	*tmp;

	tmp = get_dollor_parameter(tkn, &idx[0]);
	if (tmp == T_NULL)
		return (ft_free2((void **)&dtmp, FALSE));
	dtmp->value = ft_getenv(data, tmp);
	ft_free1((void **)&tmp);
	if (dtmp->value == T_NULL)
		dtmp->value = "";
	dtmp->idx_jump = idx[0] - idx[1];
	return (TRUE);
}

static char	*do_expansion(t_dll *tmp_dll, char *tkn, int e_len)
{
	char			*tmp[2];
	int				idx[3];
	t_dllnode		*node_ptr;

	tmp[0] = (char *)ft_calloc(ft_strlen(tkn) + e_len + 1, sizeof(char));
	if (tmp[0] == T_NULL)
		return (T_NULL);
	idx[0] = -1;
	idx[2] = -1;
	node_ptr = tmp_dll->head.back;
	while (tkn[++idx[0]] != '\0')
	{
		if (tkn[idx[0]] == -1)
		{
			tmp[1] = ((t_dollor_tmp *)(node_ptr->contents))->value;
			idx[1] = -1;
			while (tmp[1][++idx[1]] != '\0')
				tmp[0][++idx[2]] = tmp[1][idx[1]];
			idx[0] += ((t_dollor_tmp *)(node_ptr->contents))->idx_jump;
			node_ptr = node_ptr->back;
		}
		else
			tmp[0][++idx[2]] = tkn[idx[0]];
	}
	return (tmp[0]);
}
