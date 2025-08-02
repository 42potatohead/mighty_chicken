/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chicken.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zabu-bak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:59:48 by ataan             #+#    #+#             */
/*   Updated: 2025/08/02 18:00:38 by zabu-bak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

#define quacking 1

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t g_received_signal = 0; // Global variable to control the loop

void init_var(t_grand *grand)
{
    grand->chicken.input = NULL;
    grand->env.full_path = NULL;
    grand->chicken.token_count = 0;
    grand->chicken.status = 0;
    grand->saved_stdin = -3;
    grand->saved_stdout = -3;
    grand->qoutes = 0;
    grand->in_single = 0;
    grand->in_double = 0;
    grand->astatrr = malloc(sizeof(t_ASTatrr) * 1);
}



void free_tokens(t_Token *tokens, int count)
{
    if (!tokens)
        return;

    int i = 0;
    while (i < count && tokens[i].type != TK_END)
    {
        if (tokens[i].value)
            free(tokens[i].value);
        i++;
    }
    // Free the TK_END value if it exists
    if (i < count && tokens[i].type == TK_END && tokens[i].value)
        free(tokens[i].value);

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

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        g_received_signal = SIGINT;
        if (g_received_signal == SIGINT)
        printf("\n");
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

char **copy_env(char **envp)
{
    int count = 0;
    while (envp[count]) count++;

    char **new_env = malloc(sizeof(char *) * (count + 1));
    if (!new_env) return NULL;

    for (int i = 0; i < count; i++)
        new_env[i] = strdup(envp[i]);
    new_env[count] = NULL;
    return new_env;
}

void free_env(char **envp)
{
    if (!envp) return;

    int i = 0;
    while (envp[i]) {
        free(envp[i]);
        i++;
    }
    free(envp);
}

void clean_exit(t_grand *grand, t_Token *tokens, t_ASTNode *ast)
{
    free_tokens(tokens, grand->chicken.token_count);
    free_ast(ast);
    close_redirection_fds();
}

int main(int argc, char **argv, char **envp)
{
    t_grand grand;
    t_Token *tokens;
    t_ASTNode *ast;

    (void)argc;
    (void)argv;
    grand.env.envp = copy_env(envp);
    init_var(&grand);
    print_banner();
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    while (quacking)
    {
        grand.chicken.input = readline("\001\033[1;36m\002quack> \001\033[0m\002 ");
        if (!grand.chicken.input)
            break;
        grand.token_counter = 0;
        if (g_received_signal == SIGINT)
        {
            g_received_signal = 0;
            grand.chicken.status = 130;
            // clean
            // continue; // Skip to the next iteration
        }
        grand.chicken.token_count = 0;
        if (grand.chicken.input)
            tokens = lexer(grand.chicken.input, &grand);
        ast = parse_expression(&tokens, &grand);
        if (ast /*&& grand.chicken.status != 13*/)
        {
            execute(ast, &grand);
        }
        ft_printf("errno %d\n", grand.chicken.status);
        if (*grand.chicken.input)
            add_history(grand.chicken.input);

        // Free the allocated memory
        // if (i == 1)
        //     free_tokens(tokens, grand.chicken.token_count);
        // i++;
        // if(ast)
        //     free_ast(ast);
        // if (tokens)
            free_tokens(tokens, grand.chicken.token_count);
        dup2(grand.saved_stdin, 0);
        dup2(grand.saved_stdout, 1);
        free(grand.chicken.input);
    }
    free_env(grand.env.envp);
    return (0);
}


