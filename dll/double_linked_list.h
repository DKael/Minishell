/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_linked_list.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 20:16:13 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/23 14:12:56 by junehyle         ###   ########.fr       */
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
t_bool		dll_add_front(t_dll *dll, t_dllnode *node, t_dllnode *new);
t_bool		dll_add_back(t_dll *dll, t_dllnode *node, t_dllnode *new);
void		dll_swap_node_contents(t_dllnode *node1, t_dllnode *node2);
void		dll_clear(t_dll *dll, void (*del)(void *));
t_dllnode	*dll_new_node(void *contents);
int			dll_node_compare(t_dllnode *n1, t_dllnode *n2,
				int (*compare)(t_dllnode *, t_dllnode *));
t_bool		dll_del_node(t_dll *dll, t_dllnode *node, void (*del)(void *));
t_bool		dll_node_move_to_another_dll_head(t_dllnode *n,
				t_dll *dll1, t_dll *dll2);
t_bool		dll_node_move_to_head(t_dllnode *node, t_dll *dll);
t_bool		dll_node_move_to_another_dll_tail(t_dllnode *n,
				t_dll *dll1, t_dll *dll2);
t_bool		dll_node_move_to_tail(t_dllnode *node, t_dll *dll);
t_bool		dll_content_add(t_dll *dll, void *content, int mode);
t_bool		dll_content_add2(t_dll *dll, void *content,
				t_dllnode *ptr, int mode);
void		dll_print(t_dll *dll, void (*print)(void *));

#endif
