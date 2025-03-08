/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mighty_chick.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:59:48 by ataan             #+#    #+#             */
/*   Updated: 2025/02/20 13:59:48 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

#define quacking 1

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
int status = EXIT_SUCCESS;

void init_var(t_grand *grand)
{
    grand->chicken.input = NULL;
}

void print_banner(void)
{
    printf(" ██████╗██╗  ██╗██╗ ██████╗██╗  ██╗███████╗███╗   ██╗    ███████╗██╗  ██╗███████╗██╗     ██╗         ██╗   ██╗ ██╗    ██████╗\n"
    "██╔════╝██║  ██║██║██╔════╝██║ ██╔╝██╔════╝████╗  ██║    ██╔════╝██║  ██║██╔════╝██║     ██║         ██║   ██║███║   ██╔═████╗\n"
    "██║     ███████║██║██║     █████╔╝ █████╗  ██╔██╗ ██║    ███████╗███████║█████╗  ██║     ██║         ██║   ██║╚██║   ██║██╔██║\n"
    "██║     ██╔══██║██║██║     ██╔═██╗ ██╔══╝  ██║╚██╗██║    ╚════██║██╔══██║██╔══╝  ██║     ██║         ╚██╗ ██╔╝ ██║   ████╔╝██║\n"
    "╚██████╗██║  ██║██║╚██████╗██║  ██╗███████╗██║ ╚████║    ███████║██║  ██║███████╗███████╗███████╗     ╚████╔╝  ██║██╗╚██████╔╝\n"
    "╚═════╝╚═╝  ╚═╝╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝      ╚═══╝   ╚═╝╚═╝ ╚═════╝ \n");

}


void print_ast(t_ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");

    int z = 0;
    switch (node->type) {
        case NODE_COMMAND:
        printf("COMMAND:\n");
        for (int i = 0; node->args[i]; i++) printf("%d %s\n",z++, node->args[i]);
        printf("\n");
        break;
        default:
        printf("UNKNOWN NODE\n");
    }
}


void execute(t_ASTNode *node)
{
    if (!node)
    return;

    if (node->type == NODE_COMMAND)
    {
        if (fork() == 0)
        {
            execvp(node->args[0], node->args);
            if (node->last_cmd == 1)
                status = errno;
            ft_printf("status %d", status);
            perror("execvp");
            exit(status);
        }
        wait(&status);
        status = WEXITSTATUS(status);
    }
    else if (node->type == NODE_PIPE)
    {
        int fd[2];
        pipe(fd);

        if (fork() == 0)
        {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execute(node->left);
            exit(status);
        }
        if (fork() == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execute(node->right);
            exit(status);
        }
        close(fd[0]);
        close(fd[1]);
        wait(&status);
        wait(&status);
        // printf("%d\n", status);
        // if (WEXITSTATUS(status) == 2)
        //         printf("cmd not found \n");
    }
}

void execute_command(t_ASTNode *node)
{
    printf("node value %s node type %d\n----------------\n", node->args[0], node->type);
    if (node->type == NODE_COMMAND)
    {
        if (fork() == 0)
        {
            char *cmdpath = ft_strjoin(getenv("PATH"), node->args[0]);

            if(execve(cmdpath, node->args, NULL) == -1)
                printf("Command Not Found\n");
        }
        else
        {
            wait(NULL);
        }
    }
}

/*
    Creates nodes for the AST (Abstract Syntax Tree)
    left and right can be used for more complex command structures such as pipes
    eg
            NODE_PIPE
            /        \
    NODE_COMMAND   NODE_COMMAND
    ("ls", "-la")  ("grep", "main")
    NODE_PIPE acts as the "parent" — its left child is the ls command and its right child is the grep command.
*/
t_ASTNode *create_node(e_NodeType type, char **args, t_ASTNode *left, t_ASTNode *right, int last_cmd)
{
    t_ASTNode *node = malloc(sizeof(t_ASTNode));
    node->type = type;
    node->args = args;
    node->left = left;
    node->right = right;
    node->last_cmd = last_cmd;
    return (node);
}

/*
    lexer, it converts text into meaningful lexical tokens, defined by the Token enum struct
*/
t_Token *lexer(const char *input, t_grand *grand)
{
    t_Token *tokens = malloc(sizeof(t_Token) * 100);

    while(*input)
    {
        while(isspace(*input))
            input++;
        if(*input == '|')
            tokens[grand->chicken.token_count++] = (t_Token){TOKEN_PIPE, ft_strdup("|")};
        else if(*input == '>')
            tokens[grand->chicken.token_count++] = (t_Token){TOKEN_REDIRECT_OUT, ft_strdup(">")};
        else if(*input == '<')
            tokens[grand->chicken.token_count++] = (t_Token){TOKEN_REDIRECT_IN, ft_strdup("<")};
        else if(*input == '>>')
            tokens[grand->chicken.token_count++] = (t_Token){TOKEN_APPENOUT, ft_strdup("<")};
        else if(*input == '-') // cmd options
        {
            const char *start = input;
            while (*input && !isspace(*input) && !ft_strchr("|<>", *input))
                input++;
            tokens[grand->chicken.token_count++] = (t_Token){TOKEN_OPTION, strndup(start, input - start)};
        }
        else // command
        {
            const char *start = input;
            while (*input && !isspace(*input) && !ft_strchr("|<>", *input))
                input++;
            tokens[grand->chicken.token_count++] = (t_Token){TOKEN_COMMAND, strndup(start, input - start)};
        }
        if (*input)
            input++; // fix garbage
    }
    tokens[grand->chicken.token_count++] = (t_Token){TOKEN_END, NULL};
    printf("tokens %s %s %s enum: %d %d\n", tokens[0].value, tokens[1].value, tokens[2].value, (int)tokens[0].type, (int)tokens[1].type);
    return (tokens);
}

/*
    Create the AST to represent the structure of complex shell commands
    right now it does commands
*/
t_ASTNode *parse_command(t_Token **tokens)
{
    int last_cmd = 0;
    char **args = malloc(sizeof(char *) * 10);
    int arg_count = 0;

    int i = 0;
    while ((*tokens)->type == TOKEN_COMMAND || (*tokens)->type == TOKEN_OPTION)
    {
        printf("testing ; %s\n", (*tokens)->value);
        args[arg_count++] = ft_strdup((*tokens)->value);
        (*tokens)++;
        if ((*tokens)->type == TOKEN_END)
            last_cmd = 1;
        printf("this is me %s\n\n", args[i++]);
    }
    args[arg_count] = NULL;

    return (create_node(NODE_COMMAND, args, NULL, NULL, last_cmd));
}

t_ASTNode *parse_expression(t_Token **tokens)
{
    t_ASTNode *left = malloc(sizeof(t_ASTNode));

    left = parse_command(tokens);
    char **args = malloc(sizeof(char *) * 1);
    args[0] = ft_strdup("|");

    while ((*tokens)->type == TOKEN_PIPE)
    {
        e_NodeType type;
        if ((*tokens)->type == TOKEN_PIPE)
            type = NODE_PIPE;
        (*tokens)++;
        t_ASTNode *right = parse_command(tokens);
        left = create_node(type ,args , left, right, 0);
    }
    return(left);
}

void process_input(t_grand *grand)
{
    if (ft_strchr(grand->chicken.input, '\'') || ft_strchr(grand->chicken.input, '"'))
    {
        printf("so sad\n");
        exit(42);
    }
    else
    {
        grand->chicken.tokens = ft_split(grand->chicken.input, ' ');
        // exec_input(grand);
    }

}

void sigint_handler(int sig)
{
    (void)sig;
    printf("\n"); // Move to a new line
    rl_on_new_line(); // Tell readline to move to a new line
    rl_replace_line("", 0); // Clear the current input
    rl_redisplay(); // Redisplay the prompt
}

void enviro(void)
{
    char **env;
    char *env_pre;

    env_pre = getenv("PATH");
    env = ft_split(env_pre, ':');
}

int main()
{
    t_grand grand;
    t_Token *Tokens;

    init_var(&grand);
    enviro();
    print_banner();
     // Set up signal handlers
    signal(SIGINT, sigint_handler); // Handle Ctrl-C
    signal(SIGQUIT, SIG_IGN);       // Ignore Ctrl-
    setenv("potato", "chicken", 0);

    while (quacking) {
        // Display prompt and get user input
        grand.chicken.input = readline("\033[1;36mquack> \033[0m ");
        // If user pressed Ctrl+D (EOF), exit the loop
        if (!grand.chicken.input)
            break;
        grand.chicken.token_count = 0;
        Tokens = lexer((const char *)grand.chicken.input, &grand);
        t_ASTNode *ast = parse_expression(&Tokens);
        printf("%d", ast->type);
        print_ast(ast, 0);
        // printf("left = %s right = %s\n", ast->left->args[0], ast->right->args[0]);
        // execute commands and pipes turn it into where to send eg builtin commands etc
        execute(ast);
        ft_printf("errno %d\n", WEXITSTATUS(status));


        // Add input to history (optional)
        if (*grand.chicken.input)  // Avoid adding empty lines
            add_history(grand.chicken.input);

        // Free the allocated memory
        free(grand.chicken.input);
        // free(Tokens);
    }

    printf("Exiting mighty_chicken...\n");
    return 0;
}
