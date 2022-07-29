/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_imperator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 11:52:17 by slahlou           #+#    #+#             */
/*   Updated: 2022/07/29 17:41:31 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__get_file_mode(char *filename, int *fd_type)
{

	if (filename[ft_strlen(filename) + 1] == 'A')
	{
		*fd_type = 1;
		return (O_CREAT | O_WRONLY | O_APPEND);
	}
	else if (filename[ft_strlen(filename) + 1] == 'T')
	{
		*fd_type = 1;
		return (O_CREAT | O_WRONLY | O_TRUNC);
	}
	else
	{
		*fd_type = 2;
		return (O_RDONLY);
	}
}

void	__cut_useless_files(t_splcmd *parser, t_list *files)
{
	t_list	*tmp;
	t_list	*save;

	save = parser->in.stock;
	if (files != save)
	{
		while (save->next != files && files != save)
			save = save->next;
		save->next = NULL;
	}
	else
	{
		parser->in.stock = NULL;
		parser->out.stock = NULL;
	}
	while (files)
	{
		tmp = files;
		files = files->next;
		free(tmp->content);
		free(tmp);
	}
}

void	__imperial_open_files(t_splcmd *parser, int *in_fd, int *out_fd)
{
	t_list	*files;
	char	*file_name;
	int		mode;
	int		fd_type;

	files = parser->in.stock;
	while (files)
	{
		file_name = (char *)files->content;
		if (file_name)
		{
			mode = __get_file_mode(file_name, &fd_type);
			if (fd_type == 2)
				*in_fd = open(file_name, mode, 0644);
			if (fd_type == 1)
				*out_fd = open(file_name, mode, 0644);
			if (*out_fd < 0 || *in_fd < 0)
			{
				printf("\n\n\nprobleme in __imperial_open_files fd_type = %d, file_name = %s\n", fd_type, file_name);
				__cut_useless_files(parser, files);
				break ;
			}
		}
		files = files->next;
	}
}

int	__imperial_redirect( t_splcmd *parser, int *fds, int nb_pipe)
{
	(void) fds;
	(void) nb_pipe;
	//if (__imperial_open_files(in.stock, &(in.fd), &(out.fd)))
	__imperial_open_files(parser, &(parser->in.fd), &(parser->out.fd));
	// dup fds

	return (0);
}

int	__error_pipe_gen(int **pipe_army, int i)
{
	while (--i >= 0)
		close((*pipe_army)[i]);
	free(*pipe_army);
	*pipe_army = NULL;
	return (0);
}

int	*__pipe_army(t_splcmd *parser, int *tab_size)
{
	int	*pipe_army;
	int	i;
	int	nb_pipe;

	nb_pipe = 1;
	while (parser && ++nb_pipe)
		parser = parser->next;
	*tab_size = (nb_pipe) * 2;
	pipe_army = ft_calloc(sizeof(int), *tab_size + 1);
	pipe_army[0] = *tab_size;
	pipe_army++;
	i = 0;
	while (nb_pipe > 0)
	{
		if ((pipe(&pipe_army[i]) < 0))
			nb_pipe = __error_pipe_gen(&pipe_army, i);
		i += 2;
		nb_pipe--;
	}

	return (pipe_army);
}

int	__el_imperator(t_data *msh_data, t_splcmd *parser)
{
	int nb_pipe; // variable might not be usefull -> is coded inside msh_data.fds - 1

	msh_data->fds = __pipe_army(parser, &nb_pipe);
	if (!msh_data->fds)
		return (0); // check for returning error
	while (parser)
	{
		__imperial_redirect(parser, msh_data->fds, nb_pipe);
		// if (__imperial_redirect(parser->in, parser->out, fds, nb_pipe))
		// 	make_child(parser);
		//__imperial_close(parser->stock);

		parser = parser->next;
	}
	return (0); // to change after test
}
