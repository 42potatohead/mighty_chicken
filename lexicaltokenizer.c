/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicaltokenizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:11:22 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:26:31 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

/*
	to create a token, we need type and value:
	-   value is determined by the expander (after removing qoutes and
		expanding enviroment variables if neede)
	-   type can be builtin, command, pipe, redirect ...
		and is determined by category() based on token.value

*/
t_Token	create_token(t_TokenType type, char *value, int len, t_grand *grand)
{
	t_Token	token;
	char	*temp;

	(void)type;
	temp = ft_substr(value, 0, len);
	if (!temp)
		token.value = ft_strdup("");
	else
		token.value = expand_variables(temp, grand);
	token.type = catagory(temp, grand);
	if (!token.value)
	{
		perror("Failed to allocate memory for token value");
		exit(EXIT_FAILURE);
	}
	free(temp);
	return (token);
}

int	quotes(char **input, int *space, t_grand *grand)
{
	if (*input && **input == 39 && grand->qoutes != 2)
	{
		grand->qoutes = 1;
		(*space)++;
	}
	if (*input && **input == 34 && grand->qoutes != 1)
	{
		grand->qoutes = 2;
		(*space)++;
	}
	if (*space == 1)
		(*space) = -1;
	return (1);
}

void	verify_token(t_grand *grand, t_Token *tokens, char *start, char *input)
{
	char	*temp;
	int		flag;
	char	*temp2;

	grand->token_counter++;
	grand->qoutes = 0;
	flag = 0;
	temp = ft_substr(start, 0, input - start);
	if (temp[0] == '$')
		flag = 1;
	free(temp);
	temp2 = ft_substr(start, 0, input - start);
	temp = expand_variables(temp2, grand);
	if (temp && ((ft_strlen(temp) == 0 && flag != 1) || ft_strlen(temp) > 0))
		tokens[grand->chicken.token_count++] = create_token(TK_CMD, start, input
				- start, grand);
	free(temp);
	free(temp2);
}

/*
	lexer, it converts text into meaningful lexical tokens,
		defined by the token enum struct
*/
t_Token	*lexer(char *input, t_grand *grand)
{
	t_Token	*tokens;
	int		space;
	char	*start;

	tokens = malloc(sizeof(t_Token) * (count_tokens((char *)input) + 1));
	space = -1;
	while (*input)
	{
		while (ft_isspace(*input) && space != 0)
			input++;
		if (!lex_expression(tokens, grand, &input) && *input)
		{
			start = input;
			while (*input && quotes(&input, &space, grand))
			{
				if ((ft_isspace(*input) && space != 0) || (ft_strchr("|<>",
							*input) && space != 0))
					break ;
				input++;
			}
			verify_token(grand, tokens, start, input);
		}
	}
	tokens[grand->chicken.token_count++] = (t_Token){TK_END, NULL};
	return (tokens);
}
