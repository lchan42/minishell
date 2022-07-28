/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 11:12:12 by slahlou           #+#    #+#             */
/*   Updated: 2022/07/28 18:00:00 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*__remove_quote(char *str)
{
	char	quote_flag;
	int		i;
	char	*start;

	if (!str)
		return (NULL);
	quote_flag = 0;
	i = 0;
	start = str;
	while (*(str + i))
	{
		while (*(str + i) && (quote_flag == 0 || quote_flag == *(str + i))
			&& ft_strchr_b("\'\"", *(str + i)))
		{
			quote_flag ^= ((*(str + i) == '\'') * '\'') + \
			((*(str + i) == '\"') * '\"');
			i++;
		}
		*str = *(str + i);
		if (*(str + i))
			str++;
	}
	*str = '\0';
	return (start);
}

void	__expand_t_list(t_list *lst, char **env, int opt)
{
	char	buf[1];

	if (lst)
	{
		if (opt == 1 \
			&& !ft_strncmp((char *)lst->content, HD_EXP, ft_strlen_p(HD_EXP)))
			lst = lst->next;
		else if (opt == 1)
			return ;
		while (lst)
		{
			if (opt == 2)
				buf[0] = ((char *)lst->content) \
				[ft_strlen(((char *)lst->content)) + 1];
			lst->content = __expand_string((char *)lst->content, env, opt);
			 if (opt != 1)
			 	lst->content = __remove_quote((char *)lst->content);
			if (opt == 2)
			{
				((char *)lst->content) \
				[ft_strlen(((char *)lst->content)) + 1] = buf[0];
			}
			lst = lst->next;
		}
	}
}

t_splcmd	*__expand(t_splcmd *parser, char **env)
{
	t_splcmd	*parser_start;

	parser_start = parser;
	while (parser)
	{
		__expand_t_list(parser->in.stock, env, 2);
		__expand_t_list(parser->in.here_buffer, env, 1);
		__expand_t_list(parser->cmd.cmd_lst, env, 0);
		__cmdtab_init(parser->cmd.cmd_lst);
		parser = parser->next;
	}

	return (parser_start);
}
