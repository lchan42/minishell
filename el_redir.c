/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:27:07 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/07 20:40:43 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__imperial_dup_fds(t_splcmd *parser, int *fds, int fd_i)
{
	if (parser->in.fd == 0 && parser->in.type != HERE_D && fd_i == 0)
		dup2(0, fds[fd_i]);
	else if (parser->in.fd > 0 && parser->in.type != HERE_D)
	{
		dup2(parser->in.fd, fds[fd_i]);
		close(parser->in.fd);
	}
	if (!parser->out.fd)
	{
		printf("COUCOU JE SUIS LE HEREDOC dans !parser->out.fd\n");
		dup2(1, fds[fd_i + 3]);
		close(fds[fd_i + 2]);
	}
	else if (parser->out.fd != 1)
	{
		printf("COUCOU JE SUIS LE HEREDOC dans parser->out.fd != 1)\n");
		dup2(parser->out.fd, fds[fd_i + 3]);
		close(parser->out.fd);
	}
}

int	__imperial_redirect(t_splcmd *parser, int *fds, int fd_i)
{
	__imperial_open_files(parser, &(parser->in.fd), &(parser->out.fd));
	__imperial_open_heredoc(&(parser->in), fds + fd_i);
	__imperial_dup_fds(parser, fds, fd_i);
	return (0);
}
