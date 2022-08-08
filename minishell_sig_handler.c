/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_sig_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 13:21:19 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/08 13:27:48 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__signal_handler3(int sig)
{
	if (sig == SIGINT)
	{
		__ultimate_free((t_data *)g_lob_ptr, 0, 0);
		exit(0);
	}
}

void	__signal_handler2(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
}

void	__signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
