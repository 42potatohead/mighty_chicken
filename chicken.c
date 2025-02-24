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

is_cmd
{
    if (accsess(paths, token[i])
        return 1;
    else
        return 0;
}

void exec_input(t_grand *grand)
{
    int i = 0, x = 0;
    while(grand->chicken.tokens[i])
    {
        if (grand->chicken.tokens[i] == operator)
            i++;
        if (grand->chicken.tokens[i] == file)
            i++;
        if (ft_strncmp(grand->chicken.tokens[i], grand->chicken.builtins[x]) == 0)
        {
            builtin(chiocken[i]);
        }
        else if (is_cmd)
        {
            exev(grand->chicken.tokens[i]);
        }
        i++;
    }
    excve;
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
        exec_input(grand);
    }

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

    grand.chicken.builtins[0] = "pwd";
    grand.chicken.builtins[1] = "echo";
    grand.chicken.builtins[2] = "cd";
    grand.chicken.builtins[3] = "export";
    grand.chicken.builtins[4] = "unset";
    grand.chicken.builtins[5] = "env";
    grand.chicken.builtins[6] = "exit";
    grand.chicken.builtins[7] = NULL;


    init_var(&grand);
    enviro();
    print_banner();
    while (quacking) {
        // Display prompt and get user input
        grand.chicken.input = readline("quack> ");
        process_input(&grand);

        // If user pressed Ctrl+D (EOF), exit the loop
        if (!grand.chicken.input)
            break;

        // Add input to history (optional)
        if (*grand.chicken.input)  // Avoid adding empty lines
            add_history(grand.chicken.input);

        // Free the allocated memory
        free(grand.chicken.input);
    }

    printf("Exiting mighty_chicken...\n");
    return 0;
}
