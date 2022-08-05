/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 18:13:06 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/05 15:08:15 by slahlou          ###   ########.fr       */
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

int	__echo_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	int		ret;
	(void)	msh_data;

	ret = 1;
	if (parser->cmd.cmd_words)
	{
		if (opt == 0)
		{
			if (parser->out.fd == 0)
				ret = built_echo(parser->cmd.cmd_words, 1);
			else
				ret = built_echo(parser->cmd.cmd_words, msh_data->fds[3]);
		}
		else if (opt)
			ret = built_echo(parser->cmd.cmd_words, 1);
	}
	return (ret);
}

















int	__cd_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	(void) msh_data;
	(void) parser;
	(void) opt;
	printf("CD BUILTIN HELLO\n");
	return (0);
}

int	__pwd_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	(void) msh_data;
	(void) parser;
	(void) opt;
	printf("PWD BUILTIN HELLO\n");
	return (0);
}

int	__export_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	(void) msh_data;
	(void) parser;
	(void) opt;
	printf("EXPORT BUILTIN HELLO\n");
	return (0);
}

int	__unset_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	(void) msh_data;
	(void) parser;
	(void) opt;
	printf("UNSET BUILTIN HELLO\n");
	return (0);
}

int	__env_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	(void) msh_data;
	(void) parser;
	(void) opt;
	printf("ENV BUILTIN HELLO\n");
	return (0);
}























int	__get_exit_status(char **env, char **cmd_words)
{
	char	*tmp;

	if (*(cmd_words + 1))
	{
		tmp = *(cmd_words + 1);
		while (*tmp)
		{
			if (!ft_isdigit(*(tmp)))
				return (2);
			tmp++;
		}
		return (ft_atoi(*(cmd_words + 1)));
	}
	if (env)
		return (ft_atoi((*env) + 2));
	return (-1);
}

int	__exit_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	int exit_status;

	(void) opt;
	exit_status = __get_exit_status(msh_data->env, parser->cmd.cmd_words);
	__ultimate_free(msh_data, 0, 0);
	exit(exit_status);
}















void	__set_status_builtin(int ret, t_data *msh_data)
{
	char	*tmp;
	char	*tmp2;
	char	*old_status;

	if (!msh_data->env)
		return ;
	old_status = *(msh_data->env);
	if (old_status)
			free(old_status);
	tmp = ft_itoa(ret);
	tmp2 = tmp;
	tmp = ft_strjoin("?=", tmp2);
	free(tmp2);
	*(msh_data->env) = tmp;
}

void	__execve_builtin(t_data *msh_data, t_splcmd *parser, int opt)
{
	int ret;
	int	(*__u_built_funk[7])(t_data *msh_data, t_splcmd *parser, int opt);

	__u_built_funk[0] = &__echo_funk;
	__u_built_funk[1] = &__cd_funk;
	__u_built_funk[2] = &__pwd_funk;
	__u_built_funk[3] = &__export_funk;
	__u_built_funk[4] = &__unset_funk;
	__u_built_funk[5] = &__env_funk;
	__u_built_funk[6] = &__exit_funk;
	ret = __u_built_funk[parser->cmd.type - 1](msh_data, parser, opt); //parser->cmd.type - 1 a mettre a la place du 0;
	if (opt)
		exit(ret);
	else
		__set_status_builtin(ret, msh_data);
}
