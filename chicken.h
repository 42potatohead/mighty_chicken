/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chicken.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:02:19 by ataan             #+#    #+#             */
/*   Updated: 2025/02/20 14:02:19 by ataan            ###   ########.fr       */
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
    TOKEN_APPENOUT,
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
} t_ASTNode;

typedef struct s_chicken
{
    int token_count;
    char *input;
    char *builtins[8];
    char **tokens;
} t_chicken;

typedef struct s_env
{
    char **envp;
} t_env;

typedef struct s_grand
{
    t_chicken chicken;
    t_Token Token;
    t_env env;
} t_grand;

t_Token *lexer(const char *input, t_grand *grand);
t_ASTNode *parse_expression(t_Token **tokens);
t_ASTNode *parse_command(t_Token **tokens);
t_ASTNode *create_node(e_NodeType type, char **args, t_ASTNode *left, t_ASTNode *right, int last_cmd);
char *strdup_wrapper(char *(*f)(const char *), const char *s);
int chkn_cd(char **argv, char ***envp);
char *get_env_var(char **envp, const char *key);
int set_env_var(char ***envp, const char *key, const char *value);
int chkn_unset(char ***envp, char **argv);
int chkn_export(char ***envp, char **argv);
int chkn_env(char **envp);
int chkn_exit(char **argv, char ***envp);
int chkn_pwd(void);
int chkn_echo(char **argv);

# endif
