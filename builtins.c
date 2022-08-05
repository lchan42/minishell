/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 18:13:06 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/05 18:16:14 by slahlou          ###   ########.fr       */
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

int	__built_echo(char **args, int fd)
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
				ret = __built_echo(parser->cmd.cmd_words, 1);
			else
				ret = __built_echo(parser->cmd.cmd_words, msh_data->fds[3]);
		}
		else if (opt)
			ret = __built_echo(parser->cmd.cmd_words, 1);
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







int __built_pwd(int fd)
{
    char buf[PATH_MAX + 2];
	int tmp;

    if (!getcwd(buf, PATH_MAX))
    {
        perror("getcwd");
        return (errno);
    }
    else
    {
		tmp = ft_strlen(buf);
		buf[tmp] = '\n';
		buf[tmp + 1] = '\0';
		write(fd, buf, ft_strlen(buf));
        return (0);
    }
}


int	__pwd_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	int	ret;
	(void) parser;

	ret = 1;
	if (opt == 0)
	{
		if (parser->out.fd == 0)
			ret = __built_pwd(STDOUT_FILENO);
		else
			ret = __built_pwd(msh_data->fds[3]);
	}
	else if (opt)
		ret = __built_pwd(STDOUT_FILENO);
	return (ret);
}

int	__export_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	(void) msh_data;
	(void) parser;
	(void) opt;

	printf("EXPORT BUILTIN HELLO\n");
	return (0);
}


int	__get_env_size(char *env_size)
{
	int	size;

	size = 0;
	size |= (int)  *(env_size + 1) ;
	size <<= 8;
	size |= (int) *env_size;
	return (size);
}

int	__built_unset_var(char **env, char **arg)
{
	int	cnt;
	int	arg_len;
	char	*tmp_env;

	cnt = 0;
	while (*arg) // ATTENTION : il y a des cas ou arg n'est pas valide (bash: unset: `SHLVL=': not a valid identifier)
	{
		arg_len =  ft_strlen_p(*arg);
		tmp_env = __get_expand(*arg, arg_len, env);
		if (tmp_env)
		{
			*(tmp_env - (arg_len + 1)) = '\0';
 			cnt++;
 		}
		arg ++;
	}
	return (cnt);
}

int	__built_unset(t_data *msh_data, char **tmp_args, int size)
{
	int	cnt;
	(void) size;

	cnt = __built_unset_var(msh_data->env, tmp_args);
	printf("in __built_unset cnt = %d\n", cnt);
	return (0); //temporaire;
}

int	__unset_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	char	**tmp_args;
	(void)	opt;

	tmp_args = (parser->cmd.cmd_words) + 1;
	if (tmp_args)
		__built_unset(msh_data, tmp_args, __get_env_size(*((msh_data->env) - 1)));
	return (0);
}

int	__built_env(char **env, int fd)
{
	char	*tmp;

	while(env && *env)
	{
		tmp = ft_strjoin(*env, "\n");
		if (tmp)
		{
			write(fd, tmp, ft_strlen(tmp));
			free(tmp);
		}
		env++;
	}
	return (0);
}

int	__env_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	int	ret;
	(void) parser;

	ret = 1;
	if (opt == 0)
	{
		if (parser->out.fd == 0)
			ret = __built_env(msh_data->env + 1, STDOUT_FILENO);
		else
			ret = __built_env(msh_data->env + 1, msh_data->fds[3]);
	}
	else if (opt)
		ret = __built_env(msh_data->env + 1, STDOUT_FILENO);
	return (ret);
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
	int	(*__u___built_funk[7])(t_data *msh_data, t_splcmd *parser, int opt);

	__u___built_funk[0] = &__echo_funk;
	__u___built_funk[1] = &__cd_funk;
	__u___built_funk[2] = &__pwd_funk;
	__u___built_funk[3] = &__export_funk;
	__u___built_funk[4] = &__unset_funk;
	__u___built_funk[5] = &__env_funk;
	__u___built_funk[6] = &__exit_funk;
	ret = __u___built_funk[parser->cmd.type - 1](msh_data, parser, opt); //parser->cmd.type - 1 a mettre a la place du 0;
	if (opt)
		exit(ret);
	else
		__set_status_builtin(ret, msh_data);
}
