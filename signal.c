/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junehyle <junehyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 17:25:34 by junehyle          #+#    #+#             */
/*   Updated: 2023/09/23 18:04:07 by junehyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    signal_handler(int signum)
{
    if(signum == SIGINT)
    {
        printf("\n");
        rl_on_new_line();
        if (rl_on_new_line() == -1)
            exit(1);
        rl_replace_line("", 1);
        rl_redisplay();
    }
}

void    erase_signal_str(void)
{
    struct termios term;
    
    if (tcgetattr(STDIN_FILENO, &term) == -1)
    {
        perror("tcgetattr");
        exit (1);
    }

// ECHOCTL 비트 비활성화
    term.c_lflag &= ~ECHOCTL;
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
    {
        perror("tcsetattr");
        exit (1);
    }
}
