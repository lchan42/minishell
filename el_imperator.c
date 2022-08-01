/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_imperator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 11:52:17 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/01 12:29:42 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__imperial_redirect( t_splcmd *parser, int *fds, int nb_pipe)
{
	(void) fds;
	(void) nb_pipe;
	// if (__imperial_open_files(parser, &(parser->in.fd), &(parser->out.fd)))
	// 	dup_fds;
	__imperial_open_files(parser, &(parser->in.fd), &(parser->out.fd));

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
	msh_data->fds = __pipe_army(parser);
	if (!msh_data->fds)
		return (0); // check for returning error
	while (parser)
	{
		__imperial_redirect(parser, msh_data->fds, *(msh_data->fds - 1));
		// if (__imperial_redirect(parser->in, parser->out, fds, nb_pipe))
		// 	make_child(parser);
		//__imperial_close(parser->stock);
		parser = parser->next;
	}
	return (0); // to change after test
}
