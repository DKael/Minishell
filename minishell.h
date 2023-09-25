/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:25:47 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/25 20:05:17 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <dirent.h>
# include "libft/libft.h"
# include "libdll/double_linked_list.h"
# include "libgnl/get_next_line.h"
# include "libsort/quick_sort.h" 

extern int g_exit_code;

#if !defined(TRUE) && !defined(FALSE)
#define TRUE 1
#define FALSE 0
#endif
#ifndef T_NULL
#define T_NULL (void *)0
#endif

# define HD_PATH "/tmp"
# define MAX_PATH_LEN 1025
# define MAX_FD 250

typedef int t_bool;

typedef	struct stat t_stat;

typedef struct s_dir
{
	DIR				*dir;
	t_dll			*dll;
}	t_dir;

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
	char		*name;
	t_dllnode	*ptr;
}	t_srt;

typedef enum e_logic
{
	AND = 1,
	OR = 2
}	t_logic;

typedef struct s_dollor_tmp
{
	char	*value;
	int		idx_jump;
}	t_dollor_tmp;

typedef struct s_envval
{
	char	*name;
	char	*value;
}	t_envval;

typedef struct s_cmd_info
{
	int		size;
	int		redir_cnt;
	int		cp_cnt;
	t_dll	heredoc_names;
	t_bool	parentheses_flag;
	t_bool	cmd_flag;
}	t_cmd_info;

typedef struct s_data
{
	t_dll	***tkn;
	char	*cmd;
	char 	*program_name;
	char	**ao_split;
	char	**envp;
	int		ao_cnt;
	int		*pipe_cnt;
	int		**pp;
	pid_t *pid_table;
	t_logic	*logic_table;

	
	int 	exit_code;
	char	exit_code_str[4];
	
	t_dll 	envdll;
	t_dll	sorted_envdll;

	int	old_stdin;
	int	old_stdout;
	int	old_stderr;
	int	opened_fd[MAX_FD];
	char	wd[MAX_PATH_LEN];

} t_data;

// cd*.c
int		ft_chdir(char *path, t_data *data);
char	*remove_duplicate_slashs(char *str);
int		check_file(char *path);
int		ft_cd(t_data *data, char **input);
// check_syntax_error*.c
t_bool	check_syntax_error(char **cmd, int mode);
t_bool	case_lts_gts(char *cmd, int *idx);
// child.c
void child(t_data *data, int ao_idx, int pp_idx);
char **make_2d_array_from_dll(t_dll *dll);
char **make_2d_envp_from_dll(t_dll *dll);
// data_init.c
void	data_cycle_init(t_data *data);
void	data_init(t_data *data, char *program_name, char **envp);
// delete_functions.c  
void	str_delete_func(void *str);
void	envval_delete_func(void *envval);
// dll_argument_functions.c 
void	dll_env_print_func(void *content);
void	dll_export_print_func(void *content);
void	dll_str_print_func(void *content);
// dollor_sign.c
t_bool	retrieve_variable_value(t_data *data, t_dll *tkns);
char 	*ft_getenv(t_data *data, const char *name);
// echo.c
int		ft_echo(char **str);
// env.c
int		ft_env(t_data *data, char **input);
// error.c
t_bool	syntax_error_print(char *chr);
void	err_msg_print1(char *m1);
void	err_msg_print2(char *m1, char *m2);
void	err_msg_print3(char *m1, char *m2, char *m3);
void	message_exit(const char *msg, int exit_code);
// execute_builtin.c
int		is_builtin_func(char *cmd);
int		execute_builtin_func(int func_idx, char **argu_lst, t_data *data);
// exit.c
int		ft_exit(t_data *data, char **str);
// export.c
int		ft_export(t_dll *env, t_dll *s_env, char **args);
// heredoc*.c
t_bool	heredoc_make1_1(t_dll *dll, int *idx, char *del);
t_bool	heredoc_make1_2(t_dll *dll, t_dllnode *ptr, int *idx, char *del);
t_bool	heredoc_split(t_dll *dll, char *tkns);
t_bool	parentheses_heredoc(t_dll *h_names, int *tkn_idx, char *cmd);
// make_path.c
char	*make_path(char *raw_path, int mode);
// minishell.c
int		minishell(char **argv, char **envp);
// pwd.c
int		ft_pwd(t_data *data);
// redirect*.c
int		sign_redirection(t_data *data, t_dll *tkns);
t_bool	pipe_redirection(t_data *data, int ao_idx, int pp_idx);
t_bool	basic_redirection_save(t_data *data);
t_bool	basic_redirection_recover(t_data *data);
t_bool	opened_fd_close(t_data *data);
// split_cmd*.c
void	cmd_split_error(t_data *data, char *cmd, char *msg);
void	split_cmd(t_data *data, char *cmd);
t_bool	redirect_split(t_dll *dll, char *tkns);
t_bool	parentheses_split(t_dll *dll, char *tkns);
t_bool	remain_split(t_dll *dll, char *tkns);
char	*redirect_split2_0(char *tkns, t_cmd_info *tmp, int *pos,
	t_bool heredoc_flag);
void	redirect_split2_1(char *tkns, char *tmp, int *pos,
				t_bool heredoc_flag);
// srt_functions.c
int		srt_compare(void *input_lst, int idx1, int idx2);
void	srt_swap(void *input_lst, int idx1, int idx2);
// unset.c
int		ft_unset(t_dll *env, t_dll *s_env, char **args);
//util*.c
void	ignore_quote(char *cmd, int *idx);
void	ignore_parentheses(char *cmd, int *idx);
void	set_exit_code(t_data *data);
void	close_pipes(t_data *data, int num);
void	convert_minus_value_to_whitespace(char *cmd);
void	resource_free_and_exit(t_data *data, int exit_code, char *msg);
void	on_execution_part_err(t_data *data, int pp_make_cnt,
	int exit_code, char *msg);
void	total_heredoc_cnt_chk(char *cmd);
int		get_file_info(char *name, t_file_info *info, int mode);
void	*free_2d_array(void ***arr_ptr, int num);
void	*free_2d_array2(void ***arr_ptr);
void	*ft_free1(void **ptr);
t_bool	ft_free2(void **ptr, t_bool flag);
int		ft_free3(void **ptr, int return_num);
void	*free_2d_dll(t_dll ***dll_ptr, int num, void (*del)(void *));
// wildcard.c
int	wildcard(t_dll *dll);

#endif