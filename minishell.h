/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahlou <slahlou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 15:43:36 by lchan             #+#    #+#             */
/*   Updated: 2022/08/03 16:47:02 by slahlou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libraries/libft/libft.h"
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# define BUFFER_S 100000
# define T_DATA_SIZE 7
# define FIRST_PROMPT "minishell-1.0$ "
# define LEXER_PROMPT "> "
# define METACHAR "|<>" 		//dont need to interpreat';'
# define LOG_META "|&"
# define RED_META "<>"
# define HD_EXP "1\n"
# define HD_NOT_EXP "0\n"



# define AND_IF "&&"				// if left true, do right
# define OR_IF "||"					// if left faulse do right


# define SQUOTE '\''			//meta char in simple quote should be interpreted as normal char
# define DQUOTE '\"'			//same as single, expect for $ sign;
# define DOLLAR "$"				//if followed by a string, go in the env

//recode function strtok_r
// ast tree node type ; https://github.com/vorpaljs/bash-parser/blob/master/documents/ast.md


//char *readline (const char *prompt); (need to be compile with the flag -lreadline)
// rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history,

// signal, typedef void (*sighandler_t)(int);
// sigaction,
// sigemptyset,
// sigaddset,
// kill,        int kill(pid_t pid, int sig);

// getcwd,
// chdir,

// stat, get stat of a
// lstat,
// fstat,

// opendir, open directory
// readdir, read directory
// closedir, close directory

// isatty,
// ttyname,
// ttyslot,
// ioctl,

// getenv, char *getenv(const char *name) --> for the $USER

// tcsetattr,
// tcgetattr,
// tgetent,
// tgetflag,
// tgetnum,
// tgetstr,
// tgoto,
// tputs
//void	__make_token (char *str, const char delim);



/**********************************lexer struct*****************************************/
enum e_lexer_error
{
	ERR_SET_PTR = 1,
	ERR_SOLO_QUOTE,
	ERR_SYNTAX,
	ERR_SYNTAX_PIPE,
};

enum e_lexer_type_token
{
	TYPE_LEXER_WORD = 1,
	LEX_OP_LOGIC,
	LEX_OP_REDIR,
	LEX_SYNTAX_ERR
};

typedef struct s_lexer_token
{
	unsigned int	index;
	int				type;
	char			*start;
	char			*end;
	size_t			length;
}				t_lexer_token;

/**********************************parsing struct*****************************************/
enum	e_parser_io_type
{
	//STDIN,
	//STDOUT,
	IN_D = 1, // basic infile redirect
	HERE_D, // here_doc
	PIPE_IN,
	OUT_D, // basic outfile redirect
	OUT_D_APP, // append redirect mode
	PIPE_OUT
};

enum	e_parser_cmd_type
{
	BUILTOUT = 1,
	BUILTIN
};

typedef struct s_io
{
	int		type;
	char	*arg;
	int		fd;
	t_list	*stock;
	t_list	*here_buffer;
}				t_io;

typedef struct s_cmd
{
	int		type; //builtin or not
	int		size; //len of cmd_words (number of lines)
	char	**cmd_words; //contains 0 path (path + cmd ex: /usr/bin/cat) followed by one arg per line
	t_list	*cmd_lst;
}				t_cmd;

typedef struct s_splcmd
{
	t_io			in;
	t_io			out;
	t_cmd			cmd;
	struct s_splcmd	*next;
}	t_splcmd;


/*************************** main struct ****************************/
typedef struct s_data
{
	int				log_fd; //journal d erreur

	//char			*last_status;
	char			**env;
	char			**expt;

	char			*user_input;
	t_llist			*lexer;
	t_splcmd		*parser;
	int				*fds;
}t_data;




/*************************** function proto ****************************/
/*************** lexer *********************/
int			lexer_set_ptrs(char **start, char **end);
void		lexer_make(t_llist **lexer, char *str);
int			lexer_error(int error_id, t_lexer_token *current);
int			lexer_type_checker(t_llist **lexer_head, t_lexer_token *tmp_nod);

//void			lexer_add_history(t_llist *read_lst);
t_llist	*lexer(char *usr_input);



/*************** parser *********************/
t_splcmd	*__parser(t_llist *lexer);
int			__pars_io(t_io *in, t_io *out, t_llist *lexer);
t_list		*__get_stock(t_io *io, int type);
int 		__pars_cmd(t_cmd *cmd, t_llist *lexer);
char		*__here_d_unquote_limit(char *arg);

/*************** expander *******************/
t_splcmd	*__expand(t_splcmd *parser, char **env);
char		*__expand_string(char *str, char **env, int opt);
char		*__get_expand(char *start, int len, char **env);
char		*__is_expandable(char *str, char *quote_flag);
char		*__is_hd_expandable(char *str);
char		*__add_exp_to_str(char *str, char **start, char *end, char *expand);
void		__add_exp_split_str(char *str, int start, int len_expand);
char		**__cmdtab_init(t_list *cmd_lst);

/*************** imperator********************/


int		__imperial_open_files(t_splcmd *parser, int *in_fd, int *out_fd);
void	__imperial_open_heredoc(t_io *in, int *fds);
int		__el_imperator(t_data *msh_data, t_splcmd *parser);

/*************** free *********************/
//void	__ultimate_free(t_data *msh_data, int exit_opt);
void	__ultimate_free(t_data *msh_data, int exit_opt, int bambinos);
void		lexer_free(t_llist **lexer);
void		t_llist_free(t_llist **lexer);
void		__t_list_free(t_list **lst);
void		__free_parse(t_splcmd **head);

/*********** signal ************/
void	__signal_handler(int sig);
void	__signal_handler2(int sig);




/************* visual functions ****************/
void	__visual_print_tab(char **tab);
void	__visual_print_lexer(t_llist *lst);
void	__visual_print_read_lst(t_llist *usr_input);
void	__reverse_visual_print_lexer(t_llist *lst);
//void	__visual_print_splcmd(t_splcmd *head);
void	__visual_print_splcmd(t_splcmd *head, t_llist *lexer);

#endif









//test1 | test2 | test 4 | test
//<<LIMIT test1 | test2 | test 4 | test
//<<LIMIT <<a <<b test1 >LIMIT2 | <a <c test2 | test 4 >>g | test >>s
//<<LIMIT <<a <<b test1 >OUT1 | <a <c test2 >OUT2
// for lldb	//print ((t_lexer_token *)(lexer->content))->start
//test >>1 <<2 | >>3 <<4 test5
//<infile1 <infile2 <infile3 >outfile1 >outfile2 > outfile3| >>3 <<2 test5
//<<| / <<<
//<<


/********Observation
 * les messages d'erreur doivent etre imprime sur la sortie erreur
 * exit imprime exit sur sortie erreur
 * */
