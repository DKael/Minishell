/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:47 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/08 11:40:50 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
# include <limits.h>
#include <dirent.h>
#include <term.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/history.h>
#include <readline/readline.h>

#if !defined(TRUE) && !defined(FALSE)
#define TRUE 1
#define FALSE 0
#endif
#ifndef T_NULL
#define T_NULL (void *)0
#endif

typedef int t_bool;

typedef struct s_instr_info
{
	int	size;
	int	redir_cnt;
	int	heredoc_cnt;
	char	**heredoc_names;
	t_bool instr_flag;
	int		ip_cnt;
	t_bool logic_flag;
}	t_instr_info;

typedef struct s_data
{
	char ***instr;
	int instr_cnt;
	int wstatus;
	pid_t *pid_table;
	t_instr_info *instr_infos;
	char *program_name;
} t_data;

t_bool check_quote(const char *instr);
t_bool check_special_char_syntax(char **input_ptr);
char *ft_strjoin(char const *s1, char const *s2);
void split_instr(t_data *data, char *instr);
size_t ft_strlen(const char *s);
t_bool ft_isdecimal(char *str);
char *ft_strndup(char *src, size_t n);
void	*ft_calloc(size_t count, size_t size);
#endif