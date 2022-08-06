/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 17:48:40 by lchan             #+#    #+#             */
/*   Updated: 2022/08/06 17:48:49 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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

