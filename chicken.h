/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chicken.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:02:19 by ataan             #+#    #+#             */
/*   Updated: 2025/08/01 18:55:49 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHICKEN_H
# define CHICKEN_H

#include <string.h>
#include <stdio.h>
#include "./libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
# include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>

// Token types
typedef enum
{
    TOKEN_COMMAND,
    TOKEN_BUILTIN,
    TOKEN_ARGUMENT,
    TOKEN_OPTION,
    TOKEN_PIPE,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_IN,
    TOKEN_FILEIN,
    TOKEN_APPENOUT,
    TOKEN_HEREDOC,
    TOKEN_END
} e_TokenType;

// Token structure
typedef struct s_Token
{
    e_TokenType type;
    char *value;
} t_Token;

// AST node types
typedef enum
{
    NODE_COMMAND,
    NODE_BUILTIN,
    NODE_PIPE,
    NODE_REDIRECT_OUT,
    NODE_REDIRECT_IN,
    NODE_SEQUENCE
} e_NodeType;

// AST node structure
typedef struct s_ASTNode
{
    e_NodeType type;
    char **args; // For commands
    int last_cmd;
    struct s_ASTNode *left;
    struct s_ASTNode *right;
    int std_in;
    int std_out;
    int *flags; // For builtins or options
} t_ASTNode;

typedef struct s_chicken
{
    int token_count;
    char *input;
    char *builtins[8];
    char **tokens;
    int status;
} t_chicken;

typedef struct s_env
{
    char **envp;
    char **split_path;
	char *full_path;
} t_env;

typedef struct s_grand
{
    t_chicken chicken;
    t_Token Token;
    t_env env;
    char *in_file;
    char *out_file;
    int saved_stdin;
    int saved_stdout;
    int qoutes;
    int in_single;
    int in_double;
} t_grand;

t_Token *lexer(char *input, t_grand *grand);
t_ASTNode *parse_expression(t_Token **tokens, t_grand *grand);
t_ASTNode *parse_command(t_Token **tokens, t_grand *grand);
t_ASTNode *create_node(e_NodeType type, char **args, t_ASTNode *left, t_ASTNode *right, int last_cmd, int stdin, int stdout);
void execute(t_ASTNode *node, t_grand *grand);
void execute_command(t_ASTNode *node, t_grand *grand);
void call_builtin(t_ASTNode *node, t_grand *grand);
void close_wait(int fd[2], t_grand *grand);
void get_path(t_grand *grand, char *cmd);
int chkn_cd(char **argv, char ***envp);
char *get_env_var(char **envp, const char *key);
int set_env_var(char ***envp, const char *key, const char *value);
int chkn_unset(char ***envp, char **argv);
int chkn_export(char ***envp, char **argv);
int chkn_exit(char **argv);
int	chkn_pwd(char **argv, char **envp);
int chkn_echo(char **argv);
int	chkn_prnt_envp(char **argv, char **envp);
void sigint_handler(int sig);


//redirect
int handle_redirect(t_Token *tokens, t_grand *grand);
int redirect_out(char *file, t_grand *grand, int append_flag);
int redirect_in(char *file, t_grand *grand);
char	*get_env_var2(char **envp, char *key);

int    quotes(char **input, int *space, t_grand *grand);
char *expand_variables(char *value, t_grand *grand);

# endif
