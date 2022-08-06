/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 17:50:10 by lchan             #+#    #+#             */
/*   Updated: 2022/08/06 17:50:20 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
