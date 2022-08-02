/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_imperator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 11:52:17 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/02 12:49:46 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__los_bambinos_del_imperator(t_data *msh_data, t_splcmd *parser, int *fds)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		__ultimate_free(msh_data, 0, 0);
	else if (!pid)
	{
		dup2(fds[0], 0);
		close(fds[0]);
		close(fds[1]);
		dup2(fds[3], 1);
		close(fds[3]);
		if (execve(*(parser->cmd.cmd_words), parser->cmd.cmd_words, msh_data->env) < 0)
			__ultimate_free(msh_data, 0, 1); //a changer en fonction de l erreur;
	}
	close(fds[1]);
	close(fds[0]);
	return (pid);
}

void	__imperial_dup_fds(t_splcmd *parser, int *fds, int fd_i)
{
	if (parser->in.fd > 0 && parser->in.type != HERE_D)
	{
		dup2(parser->in.fd, fds[fd_i]);
		close(parser->in.fd);
	}
	if (!parser->out.fd)
		dup2(1, fds[fd_i + 3]);
	else if (parser->out.fd != 1)
	{
		dup2(parser->out.fd, fds[fd_i + 3]);
		close(parser->out.fd);
	}
}

int	__imperial_redirect(t_splcmd *parser, int *fds, int fd_i)
{
	__imperial_open_files(parser, &(parser->in.fd), &(parser->out.fd));
	__imperial_open_heredoc(&(parser->in), fds);
	__imperial_dup_fds(parser, fds, fd_i);
	return (0);
}

int	__error_pipe_gen(int **pipe_army, int i)
{
	perror("Minishell");
	while (--i >= 0)
		close((*pipe_army)[i]);
	free(*pipe_army);
	*pipe_army = NULL;
	return (0);
}

int	*__pipe_army(t_splcmd *parser)
{
	int	*pipe_army;
	int	i;
	int	nb_pipe;

	nb_pipe = 1;
	while (parser && ++nb_pipe)
		parser = parser->next;
	(nb_pipe) *= 2;
	pipe_army = ft_calloc(sizeof(int), nb_pipe + 1);
	if (pipe_army)
	{
		pipe_army[0] = nb_pipe;
		pipe_army++;
		i = 0;
		while (nb_pipe > 0)
		{
			if ((pipe(&pipe_army[i]) < 0))
				nb_pipe = __error_pipe_gen(&pipe_army, i);
			i += 2;
			nb_pipe -= 2;
		}
	}
	return (pipe_army);
}

char	*__imperial_wait(int pid, int fd_i)
{
	int		status;
	char	*ret;

	while (fd_i >= 0)
	{
		if (wait(&status) == pid)
			ret = ft_itoa(status >> 8);
		fd_i -= 2;
	}
	return (ret);
}

int	__el_imperator(t_data *msh_data, t_splcmd *parser)
{
	int	fd_i;
	int	pid;

	msh_data->fds = __pipe_army(parser);
	if (!msh_data->fds)
		return (0); // check for returning error
	fd_i = 0;
	while (parser)
	{
		__imperial_redirect(parser, msh_data->fds, fd_i);
		// if (__imperial_redirect(parser->in, parser->out, fds, nb_pipe))
		pid = __los_bambinos_del_imperator(msh_data, parser, ((msh_data->fds) + fd_i));
		printf("first --------------> pid = %d\n", pid);
		//__imperial_close(parser->stock);
		parser = parser->next;
		fd_i += 2;
	}
	msh_data->last_status = __imperial_wait(pid, fd_i);
	printf("msh_data->last_status = %s\n", msh_data->last_status);
	return (0);
}
