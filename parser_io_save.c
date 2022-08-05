/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_io_save.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 15:55:50 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/05 12:47:51 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	__save_io_arg(t_io *io)
{
	ft_lstadd_back(&(io->stock), ft_lstnew(io->arg));
}

static void	__here_d_parse_lim(t_io *io)
{
	char	*tmp;
	int		quote_flag;

	tmp = io->arg;
	quote_flag = 0;
	while (*tmp)
	{
		if (ft_strchr_b("\"\'", *tmp++))
		{
			quote_flag++;
			break ;
		}
	}
	if (!quote_flag)
		ft_lstadd_back(&(io->here_buffer), ft_lstnew(ft_strdup(HD_EXP)));
	else
	{
		tmp = io->arg;
		io->arg = __here_d_unquote_limit(io->arg);
		free(tmp);
		ft_lstadd_back(&(io->here_buffer), ft_lstnew(ft_strdup(HD_NOT_EXP)));
	}
}

// static void	__save_here_d(t_io *io)
// {
// 	//char	*limit;					if problem, remettre limit (magie noir ... )
// 	char	buf[BUFFER_S];
// 	int		read_ret;

// 	if (io->here_buffer)
// 		__t_list_free(&(io->here_buffer));
// 	__here_d_parse_lim(io);
// 	//limit = io->arg;
// 	read_ret = 1;
// 	while (read_ret)
// 	{
// 		write(1, "> ", 2);
// 		read_ret = read(0, buf, BUFFER_S);
// 		if (read_ret == 0)
// 		{
// 			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", io->arg);
// 			break ;
// 		}
// 		buf[read_ret - 1] = '\0';
// 		buf[read_ret] = '\0';
// 		if (!(ft_strncmp(buf, io->arg, read_ret)))
// 			break ;
// 		buf[read_ret - 1] = '\n';
// 		ft_lstadd_back(&(io->here_buffer), ft_lstnew(ft_strdup(buf)));
// 	}
// 	free(io->arg);
// 	io->arg = NULL;
// }

int	__child_fill_pipe(int *hd_pipe, char *limiter)
{
	int		read_ret;
	char	buf[BUFFER_S];

	read_ret = 1;
	signal(SIGINT, &__signal_handler3);
	close(hd_pipe[0]);
	while (read_ret)
	{
		write(1, "> ", 2);
		read_ret = read(0, buf, BUFFER_S);
		if (read_ret == 0)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", limiter);
			break ;
		}
		buf[read_ret - 1] = '\0';
		buf[read_ret] = '\0';
		if (!(ft_strncmp(buf, limiter, read_ret)))
			break ;
		buf[read_ret - 1] = '\n';
		write(hd_pipe[1], buf, ft_strlen_p(buf));
	}
	close(hd_pipe[1]);
	free(limiter);
	__ultimate_free((t_data *)glob_ptr, 0, 0);
	exit(0);
}

static void	__save_here_d(t_io *io)
{
	int		hd_pipe[2];
	int		pid;
	int		status;
	char	*gnl_ret;

	if (io->here_buffer)
		__t_list_free(&(io->here_buffer));
	__here_d_parse_lim(io);
	pipe(hd_pipe);
	pid = fork();
	if (pid)
	{
		signal(SIGINT, &__signal_handler2);
		if (wait(&status) == -1 || WIFSIGNALED(status))
		{
			signal(SIGINT, &__signal_handler);
			return ;
		}
		signal(SIGINT, &__signal_handler);
	}
	if (!pid)
		__child_fill_pipe(hd_pipe, io->arg);
	close(hd_pipe[1]);
	while (1)
	{
		gnl_ret = get_next_line(hd_pipe[0]);
		if (!gnl_ret)
			break ;
		ft_lstadd_back(&(io->here_buffer), ft_lstnew(gnl_ret));
	}
	close(hd_pipe[0]);
	free(io->arg);
	io->arg = NULL;

}

static void	__init_stock_mgmt(void (*stock_mgmt[7]) (t_io *io))
{
	stock_mgmt[IN_D] = &__save_io_arg;
	stock_mgmt[HERE_D] = &__save_here_d;
	stock_mgmt[OUT_D] = &__save_io_arg;
	stock_mgmt[OUT_D_APP] = &__save_io_arg;
}

t_list	*__get_stock(t_io *io, int type)
{
	void	(*stock_mgmt[7])(t_io *io);

	__init_stock_mgmt(stock_mgmt);
	stock_mgmt[type](io);
	return (io->stock);
}
