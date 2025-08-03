/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:55:01 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:28:54 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

void	init_var(t_grand *grand)
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
	grand->pflag = 0;
}

void	sigint_handler(int sig)
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
