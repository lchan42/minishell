/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 18:13:06 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/06 13:22:56 by slahlou          ###   ########.fr       */
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










void	__child_print_expt(char **tab)
{
	int		i;
	char	*tmp;
	int		var_len;

	i = -1;
	while (tab[++i])
	{
		tmp = ft_strchr(tab[i], '=');
		if (!tmp)
			printf("export %s\n", tab[i]);
		else
		{
			var_len = (tmp - tab[i]) + 1;
			printf("export %.*s\"%s\"\n", var_len, tab[i], tmp + 1);
		}
	}
}

void	__print_expt(char **tab, int fd)
{
	int		tmp_in;
	int		pid;

	if (tab)
	{
		pid = fork();
		if (!pid)
		{
			if (fd != 1)
			{
				tmp_in = dup(STDOUT_FILENO);
				dup2(fd, STDOUT_FILENO);
			}
			__child_print_expt(tab);
			if (fd != 1)
			{
				dup2(tmp_in, STDOUT_FILENO);
				dup2(STDOUT_FILENO, fd);
				close(tmp_in);
			}
			__ultimate_free((t_data *)glob_ptr, 0, 0);
			exit(0);
		}
		else
			wait(NULL);
	}
}

int	__check_syntax(char *var)
{
	if (!ft_isdigit(*var))
	{
		while (*var != '=')
		{
			if (!ft_isalnum(*var) && *var != '_')
				return (0);
			var++;
		}
		return (1);
	}
	return (0)
}

char	*__check_if_exist(char	*var, char **tab)
{
	char	*tmp;
	int		arg_len;

	tmp = NULL;
	arg_len = ft_strchr();

	arg_len =  ft_strlen_p(*arg);
	__get_e

	return (tmp);
}

void	__add_var(char	*var, char **tab)
{
	char	**new_tab;
	int		size;
	char	*exist;

	exist = __check_if_exist(var, tab);
	size = __get_env_size(tab - 1) + (exist == NULL);
}

void	__export_var(char **args, t_data *msh_data)
{
	int syntax;

	while (*args)
	{
		syntax = __check_syntax(*args);
		if (ft_strchr(*args, '=') && syntax)
			__add_var(*args, msh_data->env);
		else if (syntax)
			__add_var(*args, msh_data->expt);
		else
			write(2, "Minishell: export: not a valid identifier\n", 42);
		args++
	}
}

int	__built_export(t_data *msh_data, char **cmd_words, int fd)
{
	char	**env;
	char	**expt;

	env = (msh_data->env + 1);
	expt = msh_data->expt;
	if (!env)
		return (1);
	if (*(cmd_words + 1) == NULL)
	{
		__print_expt(env, fd);
		__print_expt(expt, fd);
	}
	else
		__export_var(*(cmd_words + 1), msh_data);
	return (0);
}

int	__export_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	(void) opt;

	int	ret;
	(void) parser;

	ret = 1;
	if (opt == 0)
	{
		if (parser->out.fd == 0)
			ret = __built_export(msh_data, parser->cmd.cmd_words, STDOUT_FILENO);
		else
			ret = __built_export(msh_data, parser->cmd.cmd_words, msh_data->fds[3]);
	}
	else if (opt)
		ret = __built_export(msh_data, parser->cmd.cmd_words, STDOUT_FILENO);
	return (ret);
}






int	__get_env_size(char *env_size) //used in all function that remalloc env
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
	free(*(env - 1));
	return (cnt);
}

char	**__built_unset(t_data *msh_data, char **env, char **tmp_args, int size)
{
	int	cnt;
	int	new_size;
	char **new_env;

	cnt = __built_unset_var(env, tmp_args);
	new_size = size - cnt;
	new_env = ft_calloc(sizeof(char *), new_size + 2);
	if (new_env)
	{
		*new_env = ft_calloc(sizeof(char), 2);
		if (*new_env)
		{
			*(*(new_env + 0) + 0) |= new_size;
			*(*(new_env + 0) + 1) |= new_size >> 8;
		}
		new_env++;
		while(*env)
		{
			if (**env)
			{
				*new_env = *env;
				new_env++;
			}
			else
				free(*env);
			env++;
		}
		*new_env = NULL;
	}
	free((msh_data->env) - 1);
	return (new_env - new_size);
	// char **tmp = new_env - new_size;
	// while (*tmp)
	// {
	// 	printf("%s\n", *tmp);
	// 	tmp++;
	// }
}

int	__unset_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	char	**tmp_args;
	(void)	opt;

	tmp_args = (parser->cmd.cmd_words) + 1;
	if (tmp_args)
		msh_data->env = __built_unset(msh_data, msh_data->env, tmp_args, __get_env_size(*((msh_data->env) - 1)));
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
