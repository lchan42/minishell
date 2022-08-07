/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 17:48:40 by lchan             #+#    #+#             */
/*   Updated: 2022/08/07 11:55:08 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*__check_wich_tab(char	*var, char **tab, int opt)
{
	int		arg_len;
	int		cmp;

	arg_len = ((opt == 1) * ft_strlen_p(var)) + ((opt == 0) * ft_strlen_c(var, '='));
	while (*tab)
	{
		cmp = ((opt == 1) * ft_strlen_p(*tab)) + ((opt == 0) * ft_strlen_c(*tab, '='));
		if (cmp == arg_len && !ft_strncmp(*tab, var, arg_len))
			return (*tab);
		tab++;
	}
	return (*tab);
}



int	__built_unset_var(t_data *msh_data, char **tab, char **arg)
{
	int	cnt;
	int	arg_len;
	char	*tmp_tab;

	cnt = 0;
	while (*arg) // ATTENTION : il y a des cas ou arg n'est pas valide (bash: unset: `SHLVL=': not a valid identifier)
	{
		arg_len =  ft_strlen_p(*arg);
		if (*(msh_data->env) == *(tab))
			tmp_tab = __check_wich_tab(*arg, tab, 0);
		else if (*(msh_data->expt) == *(tab))
			tmp_tab = __check_wich_tab(*arg, tab, 1);
		if (tmp_tab)
		{
			*(tmp_tab - (arg_len + 1)) = '\0';
 			cnt++;
 		}
		arg++;
	}
	return (cnt);
}

char	*__size_shifter(int size)
{
	char	*tmp;

	tmp = ft_calloc(sizeof(char), 2);
	if (tmp)
	{
		*(tmp + 0) |= size;
		*(tmp + 1) |= size >> 8;
	}
	return (tmp);
}


char	**__built_unset(t_data *msh_data, char **tab, char **tmp_args, int size)
{
	int	cnt;
	int	new_size;
	char **new_tab;

	cnt = __built_unset_var(msh_data, tab, tmp_args);
	if (cnt == 0)
		return (NULL);
	free(*(tab - 1));
	new_size = size - cnt;
	new_tab = ft_calloc(sizeof(char *), new_size + 2);
	if (new_tab)
	{
		*new_tab = __size_shifter(new_size);
		new_tab++;
		while(*tab)
		{
			if (**tab)
				*(new_tab++) = *tab;
			else
				free(*tab);
			tab++;
		}
		*new_tab = NULL;
	}
	return (new_tab - new_size);
}

int	__unset_funk(t_data *msh_data, t_splcmd *parser, int opt)
{
	char	**tmp_args;
	char	**tmp_env;
	char	**tmp_expt;
	(void)	opt;

	tmp_args = (parser->cmd.cmd_words) + 1;
	if (tmp_args)
	{
		tmp_env = __built_unset(msh_data, msh_data->env, tmp_args, __get_env_size(*((msh_data->env) - 1)));
		if (tmp_env)
		{
			free(msh_data->env - 1);
			msh_data->env = tmp_env;
		}
		tmp_expt =  __built_unset(msh_data, msh_data->expt, tmp_args, __get_env_size(*((msh_data->expt) - 1)));
		if (tmp_expt)
		{
			free(msh_data->expt - 1);
			msh_data->expt = tmp_expt;
		}
	}
	return (0);
}

