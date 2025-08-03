/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chicken.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:59:48 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:57:14 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

volatile sig_atomic_t	g_received_signal = 0;

char	**copy_env(char **envp)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

void	interpret(t_grand *grand)
{
	if (g_received_signal == SIGINT)
	{
		g_received_signal = 0;
		grand->chicken.status = 130;
	}
}

void	run_lexer(t_grand *grand, t_Token **tokens, t_Token **original_tokens)
{
	grand->token_counter = 0;
	grand->chicken.token_count = 0;
	if (grand->chicken.input)
	{
		*tokens = lexer(grand->chicken.input, grand);
		*original_tokens = *tokens;
	}
}

void	cracking_the_egg(int argc, char **argv, char **envp, t_grand *grand)
{
	(void)argc;
	(void)argv;
	grand->env.envp = copy_env(envp);
	init_var(grand);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	t_grand		grand;
	t_Token		*tokens;
	t_ASTNode	*ast;
	t_Token		*original_tokens;

	cracking_the_egg(argc, argv, envp, &grand);
	while (QUACKING)
	{
		grand.chicken.input = readline(COLOR_CYAN "quack> " COLOR_RESET);
		if (!grand.chicken.input)
			break ;
		interpret(&grand);
		run_lexer(&grand, &tokens, &original_tokens);
		ast = parse_expression(&tokens, &grand);
		if (ast && (ast->args[0] != NULL))
			execute(ast, &grand);
		if (*grand.chicken.input)
			add_history(grand.chicken.input);
		clean_exit(&grand, original_tokens, ast);
	}
	if (grand.chicken.input)
		clean_exit(&grand, original_tokens, ast);
	free(grand.astatrr);
	free_env(grand.env.envp);
	return (0);
}
