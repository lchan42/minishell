/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 13:10:50 by lchan             #+#    #+#             */
/*   Updated: 2022/07/16 15:52:06 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	__print_tab(char **tab)
{
	int i;

	i = 0;
	if (!tab)
	{
		printf("tab is NULL\n");
		return ;
	}
	while (*tab)
		printf("%d : %s\n", i++, *(tab++));
}

int	ft_strtablen(char **tab)
{
	char **tmp;

	if (tab)
	{
		tmp = tab;
		while (*tmp)
			tmp++;
		return (tmp - tab);
	}
	return (0);
}

char	**ft_tabdup(char **tab)
{
	int		len;
	char	**new_tab;

	if (!tab)
		return (NULL);
	len = ft_tablen(tab);
	new_tab = malloc((len + 1) * (sizeof(char **)));
	if (!new_tab)
		return (NULL);
	*(new_tab + len) = NULL;
	while (*tab)
	{
		*new_tab = ft_strdup(*tab);
		new_tab++;
		tab++;
	}
	*new_tab = NULL;
	return (new_tab - len);
}




int main(int ac, char **av, char **envp)
{
	char **env_cpy;

	//print_tab(NULL);
	//printf("tablen = %d \n", ft_tabstrlen(NULL));
	env_cpy = ft_tabdup(envp);
	print_tab(env_cpy);


	ft_free_strtab(env_cpy);
}


void	__export(char *export, char **tab);

/**********
 * export : takes a char ** list of argument.
 *
 * =, $, ', ",
 * */

