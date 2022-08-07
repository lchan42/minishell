/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 18:13:06 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/07 17:37:07 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__update_pwd(t_data *msh_data, char *oldpwd)
{
	char	buf[PATH_MAX + 2];
	char	*tmp_newpwd;
	char	*tmp_oldpwd;

	tmp_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
	msh_data->env = __add_var(tmp_oldpwd, msh_data->env, 0);
	free(tmp_oldpwd);
	if (getcwd(buf, PATH_MAX))
	{
		tmp_newpwd = ft_strjoin("PWD=", buf);
		msh_data->env = __add_var(tmp_newpwd, msh_data->env, 0);
		free(tmp_newpwd);
	}
}

int	__cd_funk(t_data *msh_data, t_splcmd *parser, int opt) //mettre a jour var env PWD
{
	int		cmd_size;
	int		err_flag;
	char	buf[PATH_MAX + 2];

	(void) opt;
	err_flag = 0;
	cmd_size = ft_strtablen(parser->cmd.cmd_words);
	if (cmd_size != 2)
	{
		write(2, "minishell: cd: invalide arguments\n", 35);
		err_flag = 1;
	}
	else
	{
		if (!getcwd(buf, PATH_MAX))
			buf[0] = '\0';
		if (chdir(*(parser->cmd.cmd_words + 1)) < 0)
		{
			perror("minishell: cd");
			err_flag = 1;
		}
		else
			__update_pwd(msh_data, buf);
	}
	return (err_flag);
}


int	__get_env_size(char *env_size)
{
	int	size;

	size = 0;
	size |= (int) *(env_size + 1) ;
	size <<= 8;
	size |= (int) *env_size;
	return (size);
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
	int	exit_status;

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
	int	ret;
	int	(*__u___built_funk[7])(t_data *msh_data, t_splcmd *parser, int opt);

	__u___built_funk[0] = &__echo_funk;
	__u___built_funk[1] = &__cd_funk;
	__u___built_funk[2] = &__pwd_funk;
	__u___built_funk[3] = &__export_funk;
	__u___built_funk[4] = &__unset_funk;
	__u___built_funk[5] = &__env_funk;
	__u___built_funk[6] = &__exit_funk;
	ret = __u___built_funk[parser->cmd.type - 1](msh_data, parser, opt);
	if (opt)
	{
		__ultimate_free(msh_data, 0, ret);
		close(0);
		close(1);
		exit(ret);
	}
	else
		__set_status_builtin(ret, msh_data);
}
