/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_io_save.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 15:55:50 by slahlou           #+#    #+#             */
/*   Updated: 2022/07/26 11:07:51 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	__save_io_arg(t_io *io)
{
	ft_lstadd_back(&(io->stock), ft_lstnew(io->arg));
}

/* old version */
/*
static void	__save_here_d(t_io *io)
{
	char	*limit;
	char	buf[BUFFER_S];
	int		read_ret;

	limit = io->arg;
	read_ret = 1;
	if (io->here_buffer)
		__t_list_free(&(io->here_buffer));
	while (read_ret)
	{
		write(1, "> ", 2);
		read_ret = read(0, buf, BUFFER_S);
		buf[read_ret] = '\0';
		if (!(ft_strncmp(buf, limit, read_ret - 1)))
			break ;
		ft_lstadd_back(&(io->here_buffer), ft_lstnew(ft_strdup(buf)));
	}
	free(io->arg);
	io->arg = NULL;
}
*/

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
			break;
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
	printf("limiteur = [%s]\n", io->arg);
}

static void	__save_here_d(t_io *io)
{
	char	*limit;
	char	buf[BUFFER_S];
	int		read_ret;

	if (io->here_buffer)
		__t_list_free(&(io->here_buffer));
	__here_d_parse_lim(io);
	limit = io->arg;
	read_ret = 1;
	while (read_ret)
	{
		write(1, "> ", 2);
		read_ret = read(0, buf, BUFFER_S);
		buf[read_ret - 1] = '\0';
		buf[read_ret] = '\0';
		if (!(ft_strncmp(buf, limit, read_ret)))
			break ;
		buf[read_ret - 1] = '\n';
		ft_lstadd_back(&(io->here_buffer), ft_lstnew(ft_strdup(buf)));
	}
	free(io->arg);
	io->arg = NULL;
}

static void	__init_stock_mgmt(void(*stock_mgmt[7])(t_io *io))
{
	stock_mgmt[IN_D] =  &__save_io_arg;
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
