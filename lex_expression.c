/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_expression.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:13:05 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 20:20:45 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

int	lex_pipe_and_append(t_Token *tokens, t_grand *grand, char **input)
{
	if (**input == '|')
	{
		tokens[grand->chicken.token_count++] = create_token(TK_PIPE, "|", 1,
				grand);
		(*input)++;
		grand->token_counter = 0;
		return (1);
	}
	else if (**input == '>' && *(*input + 1) == '>')
	{
		tokens[grand->chicken.token_count++] = create_token(TK_AOUT, ">>", 2,
				grand);
		(*input) += 2;
		grand->token_counter = 0;
		return (1);
	}
	else if (**input == '<' && *(*input + 1) == '<')
	{
		tokens[grand->chicken.token_count++] = create_token(TK_R_DILM, "<<", 2,
				grand);
		(*input) += 2;
		grand->token_counter = 0;
		return (1);
	}
	return (0);
}

int	lex_redirections(t_Token *tokens, t_grand *grand, char **input)
{
	if (**input == '>')
	{
		tokens[grand->chicken.token_count++] = create_token(TK_R_OUT, ">", 1,
				grand);
		(*input)++;
		grand->token_counter = 0;
		return (1);
	}
	else if (**input == '<')
	{
		tokens[grand->chicken.token_count++] = create_token(TK_R_IN, "<", 1,
				grand);
		(*input)++;
		grand->token_counter = -1;
		return (1);
	}
	return (0);
}

int	lex_expression(t_Token *tokens, t_grand *grand, char **input)
{
	if (lex_pipe_and_append(tokens, grand, input))
		return (1);
	if (lex_redirections(tokens, grand, input))
		return (1);
	return (0);
}
