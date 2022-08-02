/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_imperator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 11:52:17 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/02 16:40:23 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__cat_cmd_to_path(char *buf, char *path, char *cmd)
{
	int	i;

	i = 0;
	while (path && *path)
	{
		buf[i] = *path;
		i++;
		path++;
	}
	if (path != NULL)
	{
		buf[i] = '/';
		i++;
	}
	while (*cmd)
	{
		buf[i] = *cmd;
		i++;
		cmd++;
	}
	buf[i] = '\0';
}

void	__join_path(char **env, t_cmd *cmd)
{
	char	*tmp_path;
	char	**path_split;
	char	buf[BUFFER_S];
	int		i;
	int		access_ret;

	i = 0;
	tmp_path = __get_expand("PATH", 4, env);
	if (!tmp_path)
		return ;
	path_split = ft_split(tmp_path, ':');
	while (*(path_split + i))
	{
		__cat_cmd_to_path(buf, *(path_split + i), *(cmd->cmd_words));
		access_ret = access(buf, X_OK);
		if ((access_ret) == 0)
			break ;
		i++;
	}
	if (access_ret == 0)
	{
		errno = 0;
		free(*(cmd->cmd_words));
		*(cmd->cmd_words) = ft_strdup(buf);
	}
	ft_free_strtab(path_split);
}

void	__imperial_bambino(t_data *msh_data, t_splcmd *parser, int *fds)
{
	dup2(fds[0], 0);
	close(fds[0]);
	close(fds[1]);
	dup2(fds[3], 1);
	close(fds[3]);
	if (errno == 2)
		__ultimate_free(msh_data, 0, 1);
	__join_path(msh_data->env, &(parser->cmd));
	if (execve(*(parser->cmd.cmd_words), parser->cmd.cmd_words, msh_data->env) < 0)
		__ultimate_free(msh_data, 0, 127);

}

int	__los_bambinos_del_imperator(t_data *msh_data, t_splcmd *parser, int *fds)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		__ultimate_free(msh_data, 0, 0);
	else if (!pid)
		__imperial_bambino(msh_data, parser, fds);
	close(fds[1]);
	close(fds[0]);
	return (pid);
}

void	__imperial_dup_fds(t_splcmd *parser, int *fds, int fd_i)
{
	if (parser->in.fd == 0 && fd_i == 0)
		dup2(0, fds[fd_i]);
	else if (parser->in.fd > 0 && parser->in.type != HERE_D)
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

char	*__imperial_wait(int pid, int fd_i, char *old_status)
{
	int		status;
	char	*ret;

	while (fd_i >= 0)
	{
		if (wait(&status) == pid)
		{
			if (old_status)
				free(old_status);
			ret = ft_itoa(status >> 8);
		}
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
		pid = __los_bambinos_del_imperator(msh_data, parser, ((msh_data->fds) + fd_i));
		parser = parser->next;
		fd_i += 2;
	}
	msh_data->last_status = __imperial_wait(pid, fd_i, msh_data->last_status);
	return (0);
}
