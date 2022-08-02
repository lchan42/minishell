/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 17:13:14 by lchan             #+#    #+#             */
/*   Updated: 2022/08/02 16:45:51 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__init_msh(t_data *msh_data)
{
	errno = 0;
	msh_data->log_fd = 0;
	msh_data->fds = NULL;
	msh_data->last_status = NULL;
	msh_data->env = NULL;
	msh_data->expt = NULL;
	msh_data->user_input = NULL;
	msh_data->lexer = NULL;
	msh_data->parser = NULL;
}

void __set_msh_data(t_data *msh_data, char **envp)
{
	int	envp_size;
	char **env;

	__init_msh(msh_data);
	envp_size = ft_strtablen(envp);
	env = ft_calloc(sizeof(char *), envp_size + 2);
	if (env)
	{
		*env = ft_calloc(sizeof(char), 2);
		if (*env)
		{
			*(*(env + 0) + 0) |= envp_size;
			*(*(env + 0) + 1) |= envp_size >> 8;
		}
		env++;
		while(*envp)
			*(env++) = ft_strdup(*(envp++));
		*env = NULL;
	}
	msh_data->env = env - envp_size;
}

char	*ft_readline_add_history(char *prompt)
{
	char	*usr_input;

	usr_input = NULL;
	while (!usr_input)
	{
		usr_input = readline(prompt);
		if (!*usr_input)
			ft_free_char(&usr_input);
	}
	add_history(usr_input);
	return (usr_input);
}

int	main (int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;
	(void) envp;
	t_data			msh_data;

	__set_msh_data(&msh_data, envp);
	while (1)
	{
		msh_data.user_input = ft_readline_add_history(FIRST_PROMPT);
		msh_data.lexer = lexer(msh_data.user_input);
		msh_data.parser = __parser(msh_data.lexer);
		msh_data.parser = __expand(msh_data.parser, msh_data.env);
		__el_imperator(&msh_data, msh_data.parser);
		__ultimate_free(&msh_data, 3, 0);
	}
	__ultimate_free(&msh_data, 0, 0);
}
/****************** visual ******************/
 		//__visual_print_splcmd(msh_data.parser, msh_data.lexer);
		//printf("msh_data->last_status = %s\n", msh_data.last_status);

//<$USER >$_ <infile >$""USER cmd1 cmd2$USER | $$"|"   "|"       $'"USER"' $"'$USER'" | cat -eaf -qwe $USER


/*
//test'$USER' $"USER" $'USER' "$USER" $ "'$USER'" "$'USER'" $$$USER $"$'USER" "$$$USER" '$$$USER' $"$USER'''" --> input
//test$USER USER USER slahlou $ 'slahlou' $'USER' 474255slahlou $'USER 474255slahlou $$$USER slahlou''' --> bash output
//
// erreur sur :
// $'USER'
// "'$USER'"$;
// "$'USER'"
// ""$"U'S'ER'$',,,$USER$$'U'SR" --> input
// U'S'ER'$',,,slahlou474255'U'SR  --> bash output
// """U'S'ER'$',,,slahlou$$'U'SR" --> minishell output //seems OK
// "$'USER', U S E'$'R :$'$'$'$'$'$'$'$'$'$'$'$'$'$'$'$'$" --> input
// $'USER', U S E'$'R :$'$'$'$'$'$'$'$'$'$'$'$'$'$'$'$'$ --> bash output
// "$'USER', U S E'$'R :$'$'$'$'$'$'$'$'$'$'$'$'$'$'$'$'$" --> minishell output //seems OK
// $"$'USER" -->input
// $'USER -->bash output
// $'USER -->  minishell output //seems OK
// $2USER
// $2USER$2USER
// $2USER'$2USER'
// $$"USER"
*/
