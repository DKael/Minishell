#include "minishell.h"

t_bool  pipe_vs_ampersand(char *cmd, int idx)
{
	if (cmd[idx] == '|')
	{
		if (cmd[idx + 1] == '|')
			return (syntax_error_print("||"));
		else
			return (syntax_error_print("|"));
	}
	else if (cmd[idx] == '&' && cmd[idx + 1] == '&')
		return (syntax_error_print("&&"));
    return (TRUE);
}

t_bool  unexpected_eod_print(void)
{
    printf("minishell: syntax error: unexpected end of file\n");
	return (FALSE);
}

t_bool  pipe_or_ampersand(char *cmd, int idx, char **cmd_ptr, char *sliced_part)
{
    int save_idx;

	if (case_pipe_and_or(cmd_ptr, &idx) == FALSE)
		return (FALSE);
	if (*cmd_ptr != cmd)
	{
		cmd = *cmd_ptr;
		idx = save_idx;
		sliced_part = &cmd[idx];
		if (check_syntax_error(&sliced_part, 1) == FALSE)
			return (FALSE);
	}
    return (TRUE);
}

int	checking_quote(char *cmd, int idx)
{
	char	char_tmp;

	char_tmp = cmd[idx];
	while (cmd[++idx] != char_tmp)
		;
	idx++;
	return (idx);
}