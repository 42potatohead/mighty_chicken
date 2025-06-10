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
    grand->chicken.token_count = 0;
}

void free_ast(t_ASTNode *node)
{
    if (!node)
        return;

    free_ast(node->left);
    free_ast(node->right);

    if (node->args)
    {
        for (int i = 0; node->args[i]; i++)
            free(node->args[i]);
        free(node->args);
    }
    free(node);
}

void free_tokens(t_Token *tokens, int count)
{
    if (!tokens)
        return;

    int i = count * -1;
    tokens = tokens - count; // Adjust pointer to point to the last token
    while (i < 0)
    {
        printf("freeing token %s\n", tokens[i].value);
        if (tokens[i].value)
            free(tokens[i].value);
            i++;
    }
    free(tokens);
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

// if (node->type != NODE_BUILTIN)
//         {
//             char *cmdpath = ft_strjoin(getenv("PATH"), node->args[0]);
//             if (execve(cmdpath, node->args, NULL) == -1)
//             {
//                 perror("execve");
//                 exit(EXIT_FAILURE);
//             }
//         }

void execute(t_ASTNode *node, t_grand *grand)
{
    if (!node)
    return;

    if (node->type == NODE_COMMAND || node->type == NODE_BUILTIN)
    {
        printf("Executing command: %s %s\n", node->args[0], node->args[1] ? node->args[1] : "");
        // If not a built-in command, execute it as an external command
        if(node->type == NODE_BUILTIN)
        {
            printf("Executing built-in command: %s\n", node->args[0]);
            chkn_cd(node->args, &grand->env.envp); // Assuming you have a function to handle cd
        }
        else if (fork() == 0)
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
            execute(node->left, grand);
            exit(status);
        }
        if (fork() == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execute(node->right, grand);
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



// t_ASTNode *parse_expression(t_Token **tokens)
// {
//     t_ASTNode *left = malloc(sizeof(t_ASTNode));

//     left = parse_command(tokens);
//     char **args = malloc(sizeof(char *) * 1);
//     args[0] = ft_strdup("|");

//     while ((*tokens)->type == TOKEN_PIPE)
//     {
//         e_NodeType type;
//         if ((*tokens)->type == TOKEN_PIPE)
//             type = NODE_PIPE;
//         (*tokens)++;
//         t_ASTNode *right = parse_command(tokens);
//         left = create_node(type ,args , left, right, 0);
//     }
//     return(left);
// }

// t_ASTNode *parse_type(t_Token **tokens)
// {
//     if ((*tokens)->type == TOKEN_COMMAND)
//         return parse_command(tokens);
//     else if ((*tokens)->type == TOKEN_PIPE)
//     {
//         (*tokens)++;
//         return parse_expression(tokens);
//     }
//     else if((*tokens)->type == TOKEN_CD)
//         chkn_cd((*tokens)->value, NULL); // Assuming you have a function to handle cd
//     else if ((*tokens)->type == TOKEN_END)
//     {
//         return NULL; // End of input
//     }
//     else
//     {
//         printf("Unknown token type: %d\n", (*tokens)->type);
//         exit(EXIT_FAILURE);
//     }
// }

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

char **copy_env(char **envp) {
    int count = 0;
    while (envp[count]) count++;

    char **new_env = malloc(sizeof(char *) * (count + 1));
    if (!new_env) return NULL;

    for (int i = 0; i < count; i++)
        new_env[i] = strdup(envp[i]);
    new_env[count] = NULL;
    return new_env;
}

int main(int argc, char **argv, char **envp)
{
    t_grand grand;
    t_Token *Tokens;
    t_ASTNode *ast;

    grand.env.envp = copy_env(envp);
    init_var(&grand);
    print_banner();
     // Set up signal handlers
    signal(SIGINT, sigint_handler); // Handle Ctrl-C
    signal(SIGQUIT, SIG_IGN);       // Ignore Ctrl-
    setenv("potato", "chicken", 0);
    // int i = 0;
    while (quacking) {
        // Display prompt and get user input
        grand.chicken.input = readline("\033[1;36mquack> \033[0m ");
        // If user pressed Ctrl+D (EOF), exit the loop
        if (!grand.chicken.input)
            break;
        grand.chicken.token_count = 0;
        if (grand.chicken.input)
            Tokens = lexer((const char *)grand.chicken.input, &grand);
        ast = parse_expression(&Tokens);
        printf("%d", ast->type);
        print_ast(ast, 0);
        // printf("left = %s right = %s\n", ast->left->args[0], ast->right->args[0]);
        // execute commands and pipes turn it into where to send eg builtin commands etc
        if (ast && (ast->type == NODE_COMMAND || ast->type == NODE_PIPE || ast->type == NODE_BUILTIN))
        {
            execute(ast, &grand);
        }
        ft_printf("errno %d\n", WEXITSTATUS(status));


        // Add input to history (optional)
        if (*grand.chicken.input)  // Avoid adding empty lines
            add_history(grand.chicken.input);

        // Free the allocated memory
        // if (i == 1)
        //     free_tokens(Tokens, grand.chicken.token_count);
        // i++;
        // if(ast)
        //     free_ast(ast);
        free(grand.chicken.input);
    }

    printf("Exiting mighty_chicken...\n");
    return 0;
}
