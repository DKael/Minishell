/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:24:13 by hyungdki          #+#    #+#             */
/*   Updated: 2023/09/27 16:44:33 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//signal ^C받았을때 1값을 뱉음
void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		//g_exit_code = 1;
	}
}

//함수실행하면 신호받은 커맨드를 출력하지 않는다 비트연산(~)을 통해 ECHOCTL을 비활성화 시킴=> minishell이 진행되는 동안 적용
void	dont_make_command_str(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;//ECHOCTL을 비트 연산을 통해 비활성화 시킴으로서 제어 문자를 입력 했을때 내용을 출력하지않게하는 기능
	tcsetattr(STDIN_FILENO, TCSANOW, &term);//TCSANOW는 지금 바로 적용시킨다는 의미
}

void	make_command_str(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;//ECHOCTL을 비트 연산을 통해 비활성화 시킴으로서 제어 문자를 입력 했을때 내용을 출력하지않게하는 기능
	tcsetattr(STDIN_FILENO, TCSANOW, &term);//TCSANOW는 지금 바로 적용시킨다는 의미
}
