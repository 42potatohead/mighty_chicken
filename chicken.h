/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chicken.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:02:19 by ataan             #+#    #+#             */
/*   Updated: 2025/08/02 18:05:55 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHICKEN_H
# define CHICKEN_H

# include "./libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>

// token types
typedef enum e_TokenType
{
	TK_CMD,
	TK_BLTN,
	TK_PIPE,
	TK_R_OUT,
	TK_R_IN,
	TK_AOUT,
	TK_R_DILM,
	TK_END
}						t_TokenType;

// token structure
typedef struct s_Token
{
	t_TokenType			type;
	char				*value;
}						t_Token;

// AST node types
typedef enum t_NodeType
{
	NODE_COMMAND,
	NODE_BUILTIN,
	NODE_PIPE,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_IN,
}						t_NodeType;

typedef struct s_ASTatrr
{
	t_NodeType			type;
	char				**args;
	int					last_cmd;
	struct s_ASTNode	*left;
	struct s_ASTNode	*right;
	int					std_in;
	int					std_out;
}						t_ASTatrr;

// AST node structure
typedef struct s_ASTNode
{
	t_NodeType			type;
	char				**args;
	int					last_cmd;
	struct s_ASTNode	*left;
	struct s_ASTNode	*right;
	int					std_in;
	int					std_out;
}						t_ASTNode;

typedef struct s_chicken
{
	int					token_count;
	char				*input;
	char				*builtins[8];
	char				**tokens;
	int					status;
}						t_chicken;

typedef struct s_env
{
	char				**envp;
	char				**split_path;
	char				*full_path;
}						t_env;

typedef struct s_grand
{
	t_chicken			chicken;
	// t_Token				token;
	t_env				env;
	t_ASTatrr			*astatrr;
	char				*in_file;
	char				*out_file;
	int					saved_stdin;
	int					saved_stdout;
	int					qoutes;
	int					in_single;
	int					in_double;
	int					token_counter;
}						t_grand;

t_Token					*lexer(char *input, t_grand *grand);
t_ASTNode				*parse_expression(t_Token **tokens, t_grand *grand);
t_ASTNode				*parse_command(t_Token **tokens, t_grand *grand);
t_ASTNode				*create_node(t_grand *grand, t_ASTNode *left,
							t_ASTNode *right);
void					execute(t_ASTNode *node, t_grand *grand);
void					execute_command(t_ASTNode *node, t_grand *grand);
void					call_builtin(t_ASTNode *node, t_grand *grand);
void					close_wait(int fd[2], t_grand *grand);
void					get_path(t_grand *grand, char *cmd);
int						chkn_cd(char **argv, char ***envp);
char					*get_env_var(char **envp, const char *key);
int						set_env_var(char ***envp, const char *key,
							const char *value);
int						chkn_unset(char ***envp, char **argv);
int						chkn_export(char ***envp, char **argv);
int						chkn_exit(char **argv);
int						chkn_pwd(char **argv, char **envp);
int						chkn_echo(char **argv);
int						chkn_prnt_envp(char **argv, char **envp);
void					sigint_handler(int sig);

//redirect
int						handle_redirect(t_Token *tokens, t_grand *grand);
int						redirect_out(char *file, t_grand *grand,
							int append_flag);
int						redirect_in(char *file, t_grand *grand);
char					*get_env_var2(char **envp, char *key);
int						parse_in(t_Token **tokens, t_grand *grand);

int						quotes(char **input, int *space, t_grand *grand);
char					*expand_variables(char *value, t_grand *grand);
void clean_exit(t_grand *grand, t_Token *tokens, t_ASTNode *ast);
void close_redirection_fds(void);

#endif
