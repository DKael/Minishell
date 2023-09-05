/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:47 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/04 20:53:37 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <term.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>

#if !defined(TRUE) && !defined(FALSE)
#define TRUE 1
#define FALSE 0
#endif
#ifndef T_NULL
#define T_NULL (void *)0
#endif

typedef int t_bool;

typedef struct s_data
{
	char ***instr;
	int pipe_cnt;
	int wstatus;
	pid_t *pid_table;
} t_data;

t_bool check_special_char_syntax(char *input);
char *ft_strjoin(char const *s1, char const *s2);
char ***parsing(t_data *data, char *instr);
size_t ft_strlen(const char *s);
t_bool ft_isdecimal(char *str);
char *ft_strndup(char *src, size_t n);
#endif