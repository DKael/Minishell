/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard1_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 13:27:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/28 15:59:49 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

extern int	wildcard_redirection_check(t_dll *dll, t_dll *tmp_dll,
				t_dllnode **ptr);
static void	wildcard_init(t_dllnode **ptr, t_bool *first_find);
static int	wildcard2(t_dll *tmp_dll, t_dllnode **ptr,
				t_bool *first_find);
static void	wildcard3(t_dll *dll, t_dll *tmp_dll, t_dllnode **ptr);
static void	wildcard4(t_dll *dll, t_dllnode **ptr, t_bool *first_find);
extern char	*remove_duplicate_asterisk_slash(char *str);
extern int	do_convert(t_dll *dll, char *tkn);

int	wildcard(t_dll *dll)
{
	t_dllnode	*ptr[3];
	t_bool		first_find;
	t_dll		tmp_dll;
	int			result;

	dll_init(&tmp_dll);
	ptr[0] = dll->head.back;
	result = wildcard_redirection_check(dll, &tmp_dll, &ptr[0]);
	if (result != 0)
		return (result);
	wildcard_init(ptr, &first_find);
	while (ptr[0] != T_NULL && ptr[0] != &(dll->tail))
	{
		if (ft_strstr((char *)ptr[0]->contents, "*") != T_NULL)
		{
			result = wildcard2(&tmp_dll, ptr, &first_find);
			if (result == 2)
				continue ;
			else if (result != 0)
				return (result);
			wildcard3(dll, &tmp_dll, ptr);
		}
		wildcard4(dll, ptr, &first_find);
	}
	return (TRUE);
}

static void	wildcard_init(t_dllnode **ptr, t_bool *first_find)
{
	(*first_find) = FALSE;
	ptr[1] = T_NULL;
	ptr[2] = T_NULL;
}

static int	wildcard2(t_dll *tmp_dll, t_dllnode **ptr, t_bool *first_find)
{
	int	result;

	if ((*first_find) == FALSE)
	{
		(*first_find) = TRUE;
		ptr[1] = ptr[0];
		ptr[2] = ptr[0]->back;
	}
	result = do_convert(tmp_dll, (char *)ptr[0]->contents);
	if (result == 2)
	{
		ptr[0] = ptr[0]->back;
		return (2);
	}
	else if (result != 0)
	{
		dll_clear(tmp_dll, str_delete_func);
		return (result);
	}
	return (0);
}

static void	wildcard3(t_dll *dll, t_dll *tmp_dll, t_dllnode **ptr)
{
	t_dllnode	*ptr1;
	t_dllnode	*del;

	((t_cmd_info *)(dll->head.contents))->cp_cnt += tmp_dll->size;
	ptr1 = tmp_dll->tail.front;
	while (ptr1 != &(tmp_dll->head))
	{
		ptr1 = ptr1->front;
		dll_node_insert(ptr1->back, dll, ptr[0], 0);
	}
	dll_init(tmp_dll);
	if (ptr[0] != ptr[1])
	{
		del = ptr[0];
		ptr[0] = ptr[0]->front;
		dll_del_node(dll, del, str_delete_func);
		((t_cmd_info *)(dll->head.contents))->cp_cnt--;
	}
}

static void	wildcard4(t_dll *dll, t_dllnode **ptr, t_bool *first_find)
{
	ptr[0] = ptr[0]->back;
	if (ptr[0] == ptr[2])
	{
		if (ptr[0]->front != ptr[1])
		{
			dll_del_node(dll, ptr[1], str_delete_func);
			((t_cmd_info *)(dll->head.contents))->cp_cnt--;
		}
		(*first_find) = FALSE;
	}
}
