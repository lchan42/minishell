/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 10:56:55 by lchan             #+#    #+#             */
/*   Updated: 2022/07/29 10:48:19 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*__is_expandable(char *str, char *quote_flag)
{
	if (*str == '\0')
	{
		(*quote_flag) = 0;
		return (NULL);
	}
	if ((*quote_flag) == 0 || (*quote_flag) == '\'')
		(*quote_flag) ^= (*str == '\'') * '\'';
	if ((*quote_flag) == 0 || (*quote_flag) == '\"')
		(*quote_flag) ^= (*str == '\"') * '\"';
	if (*str == '$'
		&& (ft_isalnum((*(str + 1))) || ft_strchr_b("_\"\'", *(str + 1)))
		&& *(str + 1) && (*quote_flag) != '\'')
	{
		if (!(*(str + 1) == (*quote_flag)))
		{
			if (!((*quote_flag) == '\"' && *(str + 1) == '\''))
				return (str);
		}
	}
	return (__is_expandable(str + 1, quote_flag));
}

char	*__get_expand(char *start, int len, char **env)
{
	while (*env)
	{
		if (*(*env + len) == '=' && !ft_strncmp(start, *env, len))
			return (*env + len + 1);
		env++;
	}
	return (*env);
}

char	*__add_exp_to_str(char *str, char **start, char *end, char *expand)
{
	int		len_str;
	int		full_len;
	char	*tmp;

	len_str = ft_strlen_p(str);
	full_len = (len_str - 1) - (end - *start) + ft_strlen_p(expand);
	tmp = ft_calloc(sizeof(char), full_len + 2);
	if (!tmp)
		return (NULL);
	while (str != *start - 1)
		*(tmp++) = *(str++);
	str = end;
	while (expand && *expand)
		*(tmp++) = *(expand++);
	*start = tmp;
	while (*str)
		*(tmp++) = *(str++);
	free (str - len_str);
	return (tmp - full_len);
}

char	*__is_hd_expandable(char *str)
{
	while (*str)
	{
		if (*str == '$'
			&& (ft_isalnum((*(str + 1))) || ft_strchr_b("_", *(str + 1))))
			return (str);
		str++;
	}
	return (NULL);
}

void	__add_exp_split_str(char *str, int start, int len_expand)
{
	int		i;
	char	*expand_start;

	i = -1;
	expand_start = str + start;
	while (++i < len_expand)
	{
		if (*expand_start == ' ')
			*expand_start = -1;
		else if (*expand_start == '\'')
			*expand_start = -2;
		expand_start++;
	}
	// printf("in __add_exp_split_str, str = %s\n", str);
	// printf("in __add_exp_split_str, start = %s\n", str + start);
	// printf("in __add_exp_split_str end = %s\n", str + start + len_expand);
}

char	*__expand_string_bis(char **str, char **start, char **env, int opt_quote)
{
	char	*expand;
	char	*end;
	int		tmp_start;

	tmp_start = *start - *str;
	end = ++(*start);
	if (!ft_isdigit(*end))
		while (*end && (ft_isalnum(*end) || *end == '_'))
			end++;
	else
		end++;
	expand = __get_expand(*start, end - *start, env);
	*str = __add_exp_to_str(*str, start, end, expand);
	if (opt_quote == 0)
		__add_exp_split_str(*str, tmp_start, ft_strlen_p(expand));
	return (*str);
}

char	*__expand_string(char *str, char **env, int opt)
{
	char	quote_flag;
	char	*start;

	quote_flag = 0;
	start = str;
	while (start)
	{
		if (opt == 1)
			start = __is_hd_expandable(start);
		else
			start = __is_expandable(start, &quote_flag);
		if (start)
			str = __expand_string_bis(&str, &start, env, opt + quote_flag);
		else if (!start)
			quote_flag = 0;
	}
	return (str);
}
