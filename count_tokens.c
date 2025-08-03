/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:17:27 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 20:27:02 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_to_token_end(char **input)
{
	char	quote;

	while (**input && !ft_isspace(**input) && !is_metachar(**input))
	{
		if (**input == 39 || **input == 34)
		{
			quote = **input;
			(*input)++;
			while (**input && **input != quote)
				(*input)++;
			if (**input == quote)
				(*input)++;
		}
		else
			(*input)++;
	}
}

/*
	return the total number of tokens.
	tokens are sperated by whitespace, |, >, < , or <<
	qoutes don't start a new token
*/
int	count_tokens(char *input)
{
	int	count;

	count = 0;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		count++;
		if (*input && *input == '>' && *(input + 1) == '>')
			input += 2;
		else if (*input && is_metachar(*input))
			input++;
		else
			skip_to_token_end(&input);
		while (*input && ft_isspace(*input))
			input++;
	}
	return (count);
}
