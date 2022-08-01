/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_open_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 12:28:15 by lchan             #+#    #+#             */
/*   Updated: 2022/08/01 17:28:58 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	__get_file_mode(char *filename, int *fd_type)
{
	if (filename[ft_strlen(filename) + 1] == 'A')
	{
		*fd_type = 1;
		return ( O_APPEND | O_CREAT | O_WRONLY);
	}
	else if (filename[ft_strlen(filename) + 1] == 'T')
	{
		*fd_type = 1;
		return (O_TRUNC | O_CREAT | O_WRONLY);
	}
	else
	{
		*fd_type = 2;
		return (O_RDONLY);
	}
}

static void	__closeif_useless (t_list *stock, int fd_type, int in_fd, int out_fd)
{
	char	*content;
	int		fd_type_tmp;

	while (stock && stock->content)
	{
		content = (char *)stock->content;
		__get_file_mode(content, &fd_type_tmp);
		if (fd_type == fd_type_tmp)
		{
			if (fd_type == 1)
			{
				printf("closing out_fd = %d\n", in_fd);
				if (close (out_fd) == -1)
					perror("Minishell close");
			}
			if (fd_type == 2)
			{
				printf("closing in_fd = %d\n", in_fd);
				if (close (in_fd))
					perror("Minishell close");
			}
			return ;
		}
		stock = stock->next;
	}
}

static void	__cut_useless_files(t_splcmd *parser, t_list *files)
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

int	__imperial_open_files(t_splcmd *parser, int *in_fd, int *out_fd)
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
				//printf("\n\n\nprobleme in __imperial_open_files fd_type = %d, file_name = %s\n", fd_type, file_name);
				//add une fonction print error a la fin;
				__cut_useless_files(parser, files);
				errno = EPERM;
				return (0);
			}
			__closeif_useless(files->next, fd_type, *in_fd, *out_fd);
		}
		files = files->next;
	}
	return (1);
}

void	__imperial_open_heredoc(t_io *in, int *fds)
{
	t_list	*tmp;

	char	*tmp_content;

	if (in->type == HERE_D)
	{
		tmp = in->here_buffer->next;
		while (tmp)
		{
			tmp_content = (char *)(tmp->content);
			write(fds[1], tmp_content, ft_strlen_p(tmp_content));
			tmp = tmp->next;
		}
		__t_list_free(&(in->here_buffer));
		close(fds[1]);
	}
}
