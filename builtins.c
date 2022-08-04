/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 18:13:06 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/04 18:52:40 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *str)
{
	if (*str == '-')
	{
		str++;
		while (*str && *str == 'n')
			str++;
		if (!(*str))
			return (1);
	}
	return (0);
}

int	check_arg(char ***arg, int *opt_arg)
{
	int	i;

	i = 1;
	while (*((*arg) + i) && (*((*arg) + i))[0] == '-')
	{
		if (!check_option(*((*arg) + i)))
			break ;
		i++;
		(*opt_arg) = 1;
	}
	*arg += i;
	return (1);
}

void	ft_print_args(char **arg, int opt_arg, int fd)
{
	int	i;

	i = 0;
	while (*(arg + i))
	{
		write(fd, *(arg + i), ft_strlen(*(arg + i)));
		if (*(arg + (i + 1)))
			write(fd, " ", 1);
		else if (!(*(arg + (i + 1))) && !opt_arg)
			write(fd, "\n", 1);
		i++;
	}
}

int	built_echo(char **args, int fd)
{
	int		opt_arg;
	char	**arg_tmp;

	opt_arg = 0;
	arg_tmp = args;
	if (!(ft_strncmp(*(arg_tmp), "echo", ft_strlen(*arg_tmp))) \
	&& check_arg(&arg_tmp, &opt_arg))
		ft_print_args(arg_tmp, opt_arg, fd);
	else
		return (1);
	return (0);
}

void	__echo_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	int	ret;

	(void)msh_data;
	if (parser->cmd.cmd_words)
	{
		printf("ON EST DANS LE BUILTIN ECHO !!!\n\n");
		if (opt == 0)
		{
			if (parser->out.fd == 0)
				ret = built_echo(parser->cmd.cmd_words, 1);
			else
				ret = built_echo(parser->cmd.cmd_words, parser->out.fd);
			// changer env $? a la mano
		}
		else if (opt)
		{
			ret = built_echo(parser->cmd.cmd_words, 1);
			exit (ret);
		}
	}
}

void	__execve_builtin(t_data *msh_data, t_splcmd *parser, int opt)
{
	void	(*__u_free_funk[1])(t_data *msh_data, t_splcmd *parser, int opt);

	__u_free_funk[0] = &__echo_funk;

	__u_free_funk[0](msh_data, parser, opt); //parser->cmd.type - 1 a mettre a la place du 0;
}
