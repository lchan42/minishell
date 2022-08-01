/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_imperator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 11:52:17 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/01 21:06:46 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__los_bambinos_del_imperator(t_data *msh_data, t_splcmd *parser, int *fds)
{
	int	pid;

	close(fds[1]);
	pid = fork();
	if (pid)
		printf("pid = %d\n", pid);
	else
	{
		printf("coucou je suis le parent printf\n");
		fprintf(stderr, "-----------> coucou je suis le parent\n");
	}
	if (pid < 0)
	{
		perror("Forking error"); //adding ultmiate free to exit correctly
		exit (errno);
	}
	else if (!pid)
	{
		dup2(fds[0], 0);
		close(fds[0]);
		close(fds[1]);
		dup2(fds[3], 1);
		close(fds[3]);
		if (execve(*(parser->cmd.cmd_words), parser->cmd.cmd_words, msh_data->env) < 0)
		{
			perror("Execve function: error encountered");
			exit(errno);		//adding ultmiate free to exit correctly
		}
	}

	return (pid);
}

void	__imperial_dup_fds(t_splcmd *parser, int *fds, int fd_i)
{

	if (parser->in.fd > 0 && parser->in.type != HERE_D)
	{
		dup2(parser->in.fd, fds[fd_i]);
		close(parser->in.fd);
	}
	if (parser->out.fd && parser->out.fd != 1)
	{
		dup2(parser->out.fd, fds[fd_i + 3]);
		printf("OUT --> outfile %d has been dup on fds[%d] = %d\n", parser->out.fd, fd_i + 3, fds[fd_i + 3]);
		close(parser->out.fd);
	}
	if (!parser->out.fd)
	{
		dup2(1, fds[fd_i + 3]);
	}

}

int	__imperial_redirect(t_splcmd *parser, int *fds, int nb_pipe, int fd_i)
{
	(void) fds;
	(void) nb_pipe;

	// int		ret;
	// char		buf[2];

	// if (__imperial_open_files(parser, &(parser->in.fd), &(parser->out.fd)))
	//{
	//	here_doc;
	// 	dup_fds;
	//}
	__imperial_open_files(parser, &(parser->in.fd), &(parser->out.fd));
	__imperial_open_heredoc(&(parser->in), fds);
	__imperial_dup_fds(parser, fds, fd_i);

	// printf("pipe heredoc = \n");
	// while ((ret = read(fds[0], buf, 1)))
	// {
	// 	buf[ret] = '\0';
	// 	printf("%s", buf);
	// }
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
		printf("nbr pipe = %d\n", nb_pipe);
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

int	__el_imperator(t_data *msh_data, t_splcmd *parser)
{
	int	fd_i;
	int	pid;
	int	status;

	msh_data->fds = __pipe_army(parser);
	if (!msh_data->fds)
		return (0); // check for returning error
	fd_i = 0;
	while (parser)
	{
		__imperial_redirect(parser, msh_data->fds, *(msh_data->fds - 1), fd_i);
		// if (__imperial_redirect(parser->in, parser->out, fds, nb_pipe))
		pid = __los_bambinos_del_imperator(msh_data, parser, ((msh_data->fds) + fd_i));
		//__imperial_close(parser->stock);
		printf("COUCOU\n");
		parser = parser->next;
		fd_i += 2;
	}
	while (fd_i >= 0)
	{
		if (wait(&status) == pid)
		{
			msh_data->last_status = ft_itoa(status >> 8);
		}
		fd_i -= 2;
	}
	return (0); // to change after test
}
