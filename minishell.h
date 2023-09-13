/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:47 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/13 16:48:31 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <dirent.h>
# include <term.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "dll/double_linked_list.h"
# include "gnl/get_next_line.h"

#if !defined(TRUE) && !defined(FALSE)
#define TRUE 1
#define FALSE 0
#endif
#ifndef T_NULL
#define T_NULL (void *)0
#endif

typedef int t_bool;

typedef enum e_logic
{
	AND = 1,
	OR = 2
}	t_logic;

typedef struct s_dollor_tmp
{
	char *name;
	char *value;
	int idx_jump;
}	t_dollor_tmp;

typedef struct s_envval
{
	char *name;
	char *value;
}	t_envval;
typedef struct s_cmd_info
{
	int	size;
	int	redir_cnt;
	int	heredoc_cnt;
	char	**heredoc_names;
	
	t_bool	parentheses_flag;
	t_bool cmd_flag;
	int		ip_cnt;
	
}	t_cmd_info;

typedef struct s_data
{
	t_dll ***tkn;
	char **ao_split;
	int ao_cnt;
	int	*pipe_cnt;
	pid_t *pid_table;
	t_logic	*logic_table;

	char *program_name;
	unsigned int last_exit_code;
	char	last_exit_code_str[4];
	char **envp;
	t_dll envdll;
} t_data;

t_bool syntax_error_print(char *chr);
void message_exit(const char *msg, int exit_code);
void *free_2d_array(char ***arr_ptr, int num);
void *free_2d_dll(t_dll ***dll_ptr, int num, void (*del)(void *));

t_bool	check_syntax_error(char *cmd, int mode);
t_bool check_multiple_lines(const char *cmd);
t_bool check_quote_closed(const char *cmd);
t_bool check_parentheses_syntax(char *cmd);
t_bool check_dollor_braces(char *cmd);;
t_bool check_special_char_syntax(char **input_ptr);

char *ft_strjoin(char const *s1, char const *s2);
void split_cmd(t_data *data, char *cmd);
size_t ft_strlen(const char *s);
t_bool ft_isdecimal(char *str);
char *ft_strndup(char *src, size_t n);
int	ft_strcmp(const char *s1, const char *s2);
void	*ft_calloc(size_t count, size_t size);
char	*ft_itoa(int n);
t_bool	ft_isblank(char c);
char	**ft_split(char const *s, char c);
long long	ft_atoll(const char *str);
char *ft_getenv(t_data *data, const char *name);
char *get_dollor_parameter(char *cmd, int *origin_idx);
void	str_delete_func(void *log);
char	*ft_strdup(char *src);

void ignore_quote(char *cmd, int *idx);
void ignore_parentheses(char *cmd, int *idx);

void	envval_delete_func(void *str);
#endif