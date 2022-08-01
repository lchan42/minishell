/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_make.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 18:05:05 by lchan             #+#    #+#             */
/*   Updated: 2022/08/01 11:34:57 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_llist	*__lexer_mover(t_llist *lexer, int type)
{
	if (((t_lexer_token *)lexer)
		&& ((t_lexer_token *)lexer->content)->type == type)
		lexer = lexer->next;
	while (((t_lexer_token *)lexer)
		&& ((t_lexer_token *)lexer->content)->type != type)
		lexer = lexer->next;
	if (((t_lexer_token *)lexer)
		&& ((t_lexer_token *)lexer->content)->type == type)
		lexer = lexer->next;
	return (lexer);
}

static t_splcmd	*__init_splcmd_node(t_splcmd **head, t_splcmd *runner)
{
	t_splcmd	*new_node;

	new_node = ft_calloc(1, sizeof(t_splcmd));
	if (!new_node)
		return (NULL);
	if (!(*head))
		*head = new_node;
	else
		runner->next = new_node;
	return (new_node);
}

t_splcmd	*__parser(t_llist *lexer)
{
	t_splcmd	*runner;
	t_splcmd	*head;

	runner = NULL;
	head = NULL;
	while (lexer)
	{
		runner = __init_splcmd_node(&head, runner);
		if ((__pars_io(&(runner->in), &(runner->out), lexer) == -1) \
		|| __pars_cmd(&(runner->cmd), lexer) == -1)
		{
			__free_parse(&head);
			break ;
		}
		lexer = __lexer_mover(lexer, LEX_OP_LOGIC);
	}
	return (head);
}
