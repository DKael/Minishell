/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:17:54 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 21:50:10 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char		*make_name2(char *make_path, char *bf1, char *bf2);
extern void		find_front(char *tkns, int *pos, int idx);
extern void		find_back_and_calc_blank_quote(char *tkns, int *pos, int idx);
extern void		redirect_split2_1(char *tkns, char *tmp, int *pos,
					t_bool heredoc_flag);
static void		heredoc_make3(int fd, char *paste_nl);

t_bool	heredoc_split(t_dll *dll, char *tkns)
{
	int		i[5];
	char	*con;

	i[4] = -1;
	while (tkns[++i[4]] != '\0')
	{
		if (tkns[i[4]] == '<' && tkns[i[4] + 1] == '<')
		{
			find_front(tkns, i, i[4]);
			find_back_and_calc_blank_quote(tkns, i, i[4]);
			con = (char *)ft_calloc(i[1] - i[0] - i[2] - i[3] + 2,
					sizeof(char));
			if (con == T_NULL)
				return (FALSE);
			redirect_split2_1(tkns, con, i, TRUE);
			if (dll_content_add(dll, (void *)con, 0) == FALSE)
				return (ft_free2((void **)&con, FALSE));
			i[4] = i[1] - 1;
		}
		else if (tkns[i[4]] == '\"' || tkns[i[4]] == '\'')
			ignore_quote(tkns, &i[4]);
		else if (tkns[i[4]] == '(')
			ignore_parentheses(tkns, &i[4]);
	}
	return (TRUE);
}

char	*make_name(char *make_path, int idx1, int idx2, int size)
{
	char	*tmp[3];
	char	*bf1;
	char	*bf2;

	tmp[0] = ft_itoa(idx1);
	tmp[1] = ft_itoa(idx2);
	tmp[2] = ft_itoa(size);
	if (tmp[0] == T_NULL || tmp[1] == T_NULL || tmp[2] == T_NULL)
	{
		ft_free1((void **)&tmp[0]);
		ft_free1((void **)&tmp[1]);
		return (ft_free1((void **)&tmp[2]));
	}
	bf1 = ft_strjoin2(tmp[0], tmp[1], "_");
	bf2 = ft_strjoin2(tmp[2], "heredoc", ".");
	ft_free1((void **)&tmp[0]);
	ft_free1((void **)&tmp[1]);
	ft_free1((void **)&tmp[2]);
	return (make_name2(make_path, bf1, bf2));
}

static char	*make_name2(char *make_path, char *bf1, char *bf2)
{
	char	*tmp;
	char	*result;

	if (bf1 == T_NULL || bf2 == T_NULL)
	{
		ft_free1((void **)&bf1);
		return (ft_free1((void **)&bf2));
	}
	tmp = ft_strjoin2(bf1, bf2, "_");
	ft_free1((void **)&bf1);
	ft_free1((void **)&bf2);
	if (tmp == T_NULL)
		return (T_NULL);
	result = ft_strjoin2(make_path, tmp, "/");
	ft_free1((void **)&tmp);
	return (result);
}

t_bool	heredoc_make2(char *name, char *del)
{
	int		fd;

	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (FALSE);
	heredoc_make3(fd, del);
	return (TRUE);
}

void	heredoc_make3(int fd, char *del)
{
	char	*buffer;

	while (1)
	{
		buffer = readline("> ");
		if (buffer == T_NULL
			|| ft_strcmp(buffer, del) == 0)
			break ;
		write(fd, buffer, ft_strlen(buffer));
		write(fd, "\n", 1);
		ft_free1((void **)&buffer);
	}
	ft_free1((void **)&buffer);
	close(fd);
}
