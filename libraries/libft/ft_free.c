/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 14:14:27 by lchan             #+#    #+#             */
/*   Updated: 2022/07/16 14:06:26 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>

void	ft_free(void *malloc_elem)
{
	if (malloc_elem)
		free (malloc_elem);
}

void	ft_free_char(char **malloc_elem)
{
	if (malloc_elem && *malloc_elem)
	{
		free(*malloc_elem);
		*malloc_elem = NULL;
	}
}

void	ft_free_strtab(char **tab)
{
	char	**tmp;

	tmp = tab;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(tab);
}

void	ft_free_stdarg(int n, ...)
{
	va_list	ap;
	void	*ptr;

	va_start(ap, n);
	while (n--)
	{
		ptr = va_arg(ap, void *);
		ft_free(ptr);
	}
	va_end(ap);
}
