/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   el_bambino.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:26:23 by slahlou           #+#    #+#             */
/*   Updated: 2022/08/06 17:23:36 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__cat_cmd_to_path(char *buf, char *path, char *cmd)
{
	int	i;

	i = 0;
	while (path && *path)
	{
		buf[i] = *path;
		i++;
		path++;
	}
	if (path != NULL)
	{
		buf[i] = '/';
		i++;
	}
	while (cmd && *cmd)
	{
		buf[i] = *cmd;
		i++;
		cmd++;
	}
	buf[i] = '\0';
}

void	__join_path(char **env, t_cmd *cmd)
{
	char	*tmp_path;
	char	**path_split;
	char	buf[BUFFER_S];
	int		i;
	int		access_ret;

	i = -1;
	tmp_path = __get_expand("PATH=", 4, env);
	if (!tmp_path || !cmd->cmd_words)
		return ;
	path_split = ft_split(tmp_path, ':');
	while (*(path_split + (++i)))
	{
		__cat_cmd_to_path(buf, *(path_split + i), *(cmd->cmd_words));
		access_ret = access(buf, X_OK);
		if ((access_ret) == 0)
			break ;
	}
	if (access_ret == 0)
	{
		errno = 0;
		free(*(cmd->cmd_words));
		*(cmd->cmd_words) = ft_strdup(buf);
	}
	ft_free_strtab(path_split);
}

/*
void	__bambino_set_shlvl(char **env)
{
	char	buf[20];
	int		i;
	int		j;
	char	*itoa_shlvl;
	char	*shlvl;

	while (*(env) && ft_strncmp("SHLVL=", *env, 6) != 0)
		env++;
	shlvl = *env;
	if (!shlvl)
		return ;
	i = -1;
	j = 0;
	itoa_shlvl = NULL;
	while (shlvl[++i] && !ft_isdigit(shlvl[i]))
		buf[i] = shlvl[i];
	if (ft_isdigit(shlvl[i]))
		itoa_shlvl = ft_itoa(ft_atoi(shlvl + i) + 1);
	if (itoa_shlvl)
		while (itoa_shlvl[j])
			buf[i++] = itoa_shlvl[j++];
	buf[i] = '\0';
	free(itoa_shlvl);
	free(*env);
	*env = ft_strdup(buf);
}
*/
void	__imperial_bambino(
	t_data *msh_data, t_splcmd *parser, int *fds, int left_size)
{
	dup2(fds[0], 0);
	dup2(fds[3], 1);
	while (left_size--)
		close(*(fds++));
	if (errno == 2)
		__ultimate_free(msh_data, 0, 1);
	__join_path(msh_data->env, &(parser->cmd));
	//if (msh_data->env)
	//	__bambino_set_shlvl(msh_data->env);
	if (parser->cmd.type == CMD_ERR)
	{
		close(0);
		close(1);
		__ultimate_free(msh_data, 0, 1);
	}
	if (execve(
			*(parser->cmd.cmd_words),
			parser->cmd.cmd_words, msh_data->env + 1) < 0)
	{
		close(0);
		close(1);
		__ultimate_free(msh_data, 0, 127);
	}
}

int	__cmd_is_builtin(t_splcmd *parser)
{
	int			i;
	char		*cmd_wds;
	const char	*builtin_dico[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};

	if (parser && parser->cmd.cmd_words)
	{
		i = 0;
		cmd_wds = *(parser->cmd.cmd_words);
		while (*(builtin_dico + i))
		{
			if (!ft_strncmp(cmd_wds, *(builtin_dico + i), ft_strlen_p(*(builtin_dico + i))))
			{
				parser->cmd.type = i + 1;
				return (1);
			}
			i++;
		}
	}
	return (0);
}

int	__lonely_builtin(t_data *msh_data, t_splcmd *parser, int *fds, int size)
{

	if (__cmd_is_builtin(parser) && size == 4)
	{
		__execve_builtin(msh_data, parser, 0);
		close(fds[0]);
		close(fds[1]);
		close(fds[2]);
		close(fds[3]);
		return (1);
	}
	return (0);
}


int	__los_bambinos_del_imperator(
	t_data *msh_data, t_splcmd *parser, int *fds, int left_size)
{
	int	pid;

	pid = 0;
	if (!__lonely_builtin(msh_data, parser, msh_data->fds, *((msh_data->fds) - 1)))
	{
		pid = fork();
		if (pid > 0)
			signal(SIGINT, &__signal_handler2);
		if (pid < 0)
			__ultimate_free(msh_data, 0, 0);
		else if (!pid)
		{
			signal(SIGINT, SIG_DFL);
			if (!parser->cmd.cmd_words)
				parser->cmd.type = CMD_ERR;
			__imperial_bambino(msh_data, parser, fds, left_size);
		}
		close(fds[1]);
		close(fds[0]);
	}
	return (pid);
}
