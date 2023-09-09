/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:16:13 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 01:40:45 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOUBLE_LINKED_LIST_H
# define DOUBLE_LINKED_LIST_H

# include <stdlib.h>

# if !defined(TRUE) && !defined(FALSE)
#  define TRUE 1
#  define FALSE 0
# endif
# ifndef T_NULL
#  define T_NULL (void *)0
# endif

typedef int	t_bool;

typedef struct s_dllnode
{
	struct s_dllnode	*front;
	struct s_dllnode	*back;
	void				*contents;
}	t_dllnode;

typedef struct s_dll
{
	t_dllnode	head;
	t_dllnode	tail;
	int			size;
}	t_dll;

void		dll_init(t_dll *dll);
void		dll_add_head(t_dll *dll, t_dllnode *new);
void		dll_add_tail(t_dll *dll, t_dllnode *new);
t_bool		dll_is_in(t_dll *dll, t_dllnode *check);
t_dllnode	*dll_find(t_dll *dll, void *contents, t_bool (*f)(void *, void *));
void		dll_add_front(t_dll *dll, t_dllnode *node, t_dllnode *new);
void		dll_add_back(t_dll *dll, t_dllnode *node, t_dllnode *new);
void		dll_swap_node(t_dllnode *node1, t_dllnode *node2);
void		dll_clear(t_dll *dll, void (*del)(void *));
t_dllnode	*dll_new_node(void *contents);
void		dll_del_node(t_dll *dll, t_dllnode *node, void (*del)(void *));
int			dll_node_compare(t_dllnode *n1, t_dllnode *n2,
				int (*compare)(t_dllnode *, t_dllnode *));
void		dll_node_move_to_another_dll_head(t_dllnode *n,
				t_dll *dll1, t_dll *dll2);
void		dll_node_move_to_another_dll_tail(t_dllnode *n,
				t_dll *dll1, t_dll *dll2);

#endif
