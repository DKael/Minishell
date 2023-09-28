/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:23:40 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/24 19:51:27 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1025
# endif

# define CLEAN_FD_ONLY	0
# define CLEAN_ALL 		1

# include <unistd.h>
# include <stdlib.h>

# if !defined(TRUE) && !defined(FALSE)
#  define TRUE 1
#  define FALSE 0
# endif
# ifndef T_NULL
#  define T_NULL (void *)0
# endif

typedef struct s_fd_node
{
	int					fd;
	int					start;
	int					end;
	char				bf[BUFFER_SIZE];
	int					read;
	struct s_fd_node	*next;
}	t_fd_node;

typedef struct s_lnode
{
	int				size;
	char			bf[BUFFER_SIZE];
	struct s_lnode	*next;
}	t_lnode;

typedef struct s_gnldata
{
	t_fd_node	*it;
	t_fd_node	*it_back;
	t_fd_node	**fd_start;
	t_lnode		node;
}	t_gnldata;

char	*get_next_line(int fd);
char	*get_buffer_remains(t_gnldata *data);
char	*case_nl_eof(t_gnldata *data, int idx, t_fd_node *it);
char	*read_lines1(t_gnldata *data);
char	*read_lines2(t_gnldata *data, t_lnode **pos);
void	*gnl_memmove(void *dst, const void *src, size_t n);
void	*gnl_lstclear(t_lnode **lst);
char	*do_concat(t_lnode *tstart);
void	*free_fd_and_node(t_gnldata *data, int flag);
void	parse(t_gnldata *data, int *idx, t_lnode **pos, t_lnode **tmake);

#endif
