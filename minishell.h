/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:47 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/23 20:19:31 by junehyle         ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "dll/double_linked_list.h"
# include "gnl/get_next_line.h"
# include "quick_sort.h" 


#if !defined(TRUE) && !defined(FALSE)
#define TRUE 1
#define FALSE 0
#endif
#ifndef T_NULL
#define T_NULL (void *)0
#endif

# define hd_path "/tmp"

typedef int t_bool;
typedef	struct stat t_stat;

typedef	enum s_file_type
{
	REGULAR_FILE,
	DIRECTORY,
	SYMBOLIC_LINK,
	ETC
}	t_file_type;
typedef struct s_file_info
{
	t_stat		bf;
	t_file_type type;
	int			mode;
}	t_file_info;

typedef	struct s_srt
{
	char *name;
	t_dllnode *ptr;
}	t_srt;

typedef enum e_logic
{
	AND = 1,
	OR = 2
}	t_logic;

typedef	struct s_child_data
{
	
}	t_child_data;

typedef struct s_dollor_tmp
{
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
	t_dll	heredoc_names;
	
	t_bool	parentheses_flag;
	t_bool cmd_flag;
	int		cp_cnt;
	
}	t_cmd_info;

typedef struct s_data
{
	char	*cmd;
	t_dll ***tkn;
	char **ao_split;
	int ao_cnt;
	int	*pipe_cnt;
	int		**pp;
	pid_t *pid_table;
	t_logic	*logic_table;

	char *program_name;
	int last_exit_code;
	char	last_exit_code_str[4];
	char **envp;
	t_dll 	envdll;
	t_dll	sorted_envdll;

	int	old_stdin;
	int	old_stdout;
	int	old_stderr;
	int	opened_fd[256];
	char	wd[4096];

} t_data;

void dll_str_print_func(void *content);
void dll_env_print_func(void *content);
void dll_export_print_func(void *content);

void	close_pipes(t_data *data, int num);
int	is_builtin_func(char *cmd);
int execute_builtin_func(int func_idx, char **argu_lst, t_data *data);

void resource_free_and_exit(t_data *data, int exit_code, char *msg);
//error.c
t_bool syntax_error_print(char *chr);
void	err_msg_print1(char *m1);
void	err_msg_print2(char *m1, char *m2);
void	err_msg_print3(char *m1, char *m2, char *m3);
void message_exit(const char *msg, int exit_code);

void *free_2d_array(void ***arr_ptr, int num);
void *free_2d_array2(void ***arr_ptr);
void *free_2d_dll(t_dll ***dll_ptr, int num, void (*del)(void *));

//check_syntax_error1.c
t_bool	check_special_char_syntax(char **input_ptr);
//check_syntax_error2.c
t_bool	comb_redirect(char *cmd, int *idx);
t_bool	check_redirect(char *cmd, char *temp);
t_bool	case_lts_gts(char *cmd, int *idx);
//syntax_util.c
t_bool  pipe_vs_ampersand(char *cmd, int idx);
t_bool  unexpected_eod_print(void);
t_bool  pipe_or_ampersand(char *cmd, int idx, char **cmd_ptr, char *sliced_part);
int		checking_quote(char *cmd, int idx);

t_bool	check_syntax_error(char **cmd, int mode);
t_bool check_multiple_lines(const char *cmd);
t_bool check_quote_closed(const char *cmd);
t_bool check_parentheses_syntax(char *cmd);
t_bool check_dollor_braces(char *cmd);;


char *ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin2(char const *s1, char const *s2, char *between);
void split_cmd(t_data *data, char *cmd);
size_t ft_strlen(const char *s);
t_bool ft_isdecimal(char *str);
t_bool	ft_isndecimal(char *str, int n);
char *ft_strndup(char *src, size_t n);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_itoa(int n);
int	ft_atoi(const char *str);
t_bool	ft_isblank(char c);
char	**ft_split(char const *s, char c);
long long	ft_atoll(const char *str);
char	*ft_strstr(char *str, char *to_find);
size_t	ft_strlcpy(char *dst, const char *src, size_t dsize);
char	*ft_strcpy(char *dest, char *src);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
void	ft_putnbr_fd(int n, int fd);

t_bool	retrieve_variable_value(t_data *data, t_dll *tkns);
char *ft_getenv(t_data *data, const char *name);
char *get_dollor_parameter(char *cmd, int *origin_idx);

void	str_delete_func(void *log);
char	*ft_strdup(char *src);

void ignore_quote(char *cmd, int *idx);
void ignore_parentheses(char *cmd, int *idx);

void	envval_delete_func(void *str);

t_bool	heredoc_make1_1(t_dll *dll, int *idx, char *del);
t_bool	heredoc_make1_2(t_dll *dll, t_dllnode *ptr, int *idx, char *del);


void redirect_split2_1(char *tkns, char *tmp, int *pos, t_bool heredoc_flag);
void redirect_split2_2(char *tkns, char *tmp, int *pos, t_bool heredoc_flag);
void find_back_and_calc_blank_quote(char *tkns, int *pos, int idx);
void find_front(char *tkns, int *pos, int idx);

void *ft_free1(void **ptr);
t_bool ft_free2(void **ptr, t_bool flag);

void	child(t_data *data, int ao_idx, int pp_idx);

int sign_redirection(t_data *data, t_dll *tkns);
t_bool pipe_redirection(t_data *data, int ao_idx, int pp_idx);
t_bool basic_redirection_save(t_data *data);
t_bool basic_redirection_recover(t_data *data);
void	opened_fd_close(t_data *data);
int get_file_info(char *name, t_file_info *info, int mode);

char **make_2d_array_from_dll(t_dll *dll);
char **make_2d_envp_from_dll(t_dll *dll);
//signal
void	signal_handler(int signum);
void	erase_signal_str(void);
int	ft_echo(char **input);
int	ft_cd(t_data *data, t_dll *env, char **input);
int	ft_export(t_dll *env, t_dll *s_env, char **args);
int	ft_unset(t_dll *env, t_dll *s_env, char **args);
int	ft_pwd(t_data *data);
int	ft_exit(char **input);
int	ft_env(t_data *data);

#endif