#include "chicken.h"

int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

int is_metachar(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

void skip_to_token_end(char **input)
{
    char quote;

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
int count_tokens(char *input)
{
    int count;

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
    return count;
}

int is_builtin(char *value)
{
    if(!ft_strncmp(value, "cd", ft_strlen(value)) ||
        !ft_strncmp(value, "echo", ft_strlen(value)) ||
        !ft_strncmp(value, "env", ft_strlen(value)) ||
        !ft_strncmp(value, "exit", ft_strlen(value)) ||
        !ft_strncmp(value, "export", ft_strlen(value)) ||
        !ft_strncmp(value, "unset", ft_strlen(value)) ||
        !ft_strncmp(value, "pwd", ft_strlen(value)))
        return (EXIT_SUCCESS);
    else
        return (EXIT_FAILURE);
}

int catagory(char *value,t_grand *grand)
{
    printf("token_counter %d\n", grand->token_counter);
    if (is_builtin(value) == EXIT_SUCCESS && grand->token_counter == 1)
        return (TK_BLTN);
    else if (!ft_strncmp(value, "|", 1))
        return (TK_PIPE);
    else if (!ft_strncmp(value, ">", 1))
        return (TK_R_OUT);
    else if (!ft_strncmp(value, "<", 1))
        return (TK_R_IN);
    else if (!ft_strncmp(value, ">>", 2))
        return (TK_AOUT);
    else
        return (TK_CMD);
}

/*
    to create a token, we need type and value:
    -   value is determined by the expander (after removing qoutes and
        expanding enviroment variables if neede)
    -   type can be builtin, command, pipe, redirect ...
        and is determined by category() based on token.value

*/
t_Token create_token(t_TokenType type, char *value, int len, t_grand *grand)
{
    t_Token token;
    char *temp;
    (void)type; //check why is this used

    temp = ft_substr(value, 0, len);

    if(!temp)
        token.value = ft_strdup("");
    else
        token.value = expand_variables(temp, grand);
    printf("token.v = %s, len = %d, type = %d\n", token.value, len, type);

    token.type = catagory(temp, grand);
    if (!token.value) //should this chck for value or type?
    {
        perror("Failed to allocate memory for token value");
        exit(EXIT_FAILURE);
    }
    printf("token value %s type %d\n", token.value, token.type);
    free(temp);
    return (token);
}

int lex_expression(t_Token *tokens, t_grand *grand, char **input)
{
    if(**input == '|')
    {
        tokens[grand->chicken.token_count++] = create_token(TK_PIPE, "|", 1, grand);
        (*input)++;
        grand->token_counter = 0;
        return (1);
    }
    else if(**input == '>' && *(*input + 1) == '>')
    {
        tokens[grand->chicken.token_count++] = create_token(TK_AOUT, ">>", 2, grand);
        (*input) += 2; // Skip the second '>'
        grand->token_counter = 0;
        return (1);
    }
    else if (**input == '<' && *(*input + 1) == '<')
    {
        tokens[grand->chicken.token_count++] = create_token(TK_R_DILM, "<<", 2, grand);
        (*input) += 2; // Skip the second '<'
        grand->token_counter = 0;
        return (1);
    }
    else if(**input == '>')
    {
        tokens[grand->chicken.token_count++] = create_token(TK_R_OUT, ">", 1, grand);
        (*input)++;
        grand->token_counter = 0;
        return (1);
    }
    else if(**input == '<')
    {
        tokens[grand->chicken.token_count++] = create_token(TK_R_IN, "<", 1, grand);
        (*input)++;
        grand->token_counter = -1;
        return (1);
    }
    return (0);
}

int    quotes(char **input, int *space, t_grand *grand)
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

void verify_token(t_grand *grand, t_Token *tokens, char *start, char *input)
{
    char *temp;
    int flag;

    flag = 0;
    temp = ft_substr(start, 0, input - start);
    if(temp[0] == '$')
        flag = 1;
    free(temp);
    temp = expand_variables(ft_substr(start, 0 , input - start), grand);
    if(temp && (ft_strlen(temp) == 0 && flag != 1) || ft_strlen(temp) > 0)
        tokens[grand->chicken.token_count++] = create_token(TK_CMD, start, input - start, grand);

}
/*
    lexer, it converts text into meaningful lexical tokens, defined by the token enum struct
*/
t_Token *lexer(char *input, t_grand *grand)
{
    t_Token *tokens;
    int space;

    tokens = malloc(sizeof(t_Token) * (count_tokens((char *)input) + 1));
    space = -1;
    while(*input)
    {
        while(isspace(*input) && space != 0)
            input++;
        if(!lex_expression(tokens, grand, &input) && *input) // command
        {
            char *start = input;
            while (*input && quotes(&input, &space, grand))
            {
                printf("%c\n", *input);
                printf("%d\n", space);
                if((isspace(*input) && space != 0) || (ft_strchr("|<>", *input) && space != 0))
                    break;
                input++;
            }
            grand->token_counter++;
            grand->qoutes = 0; //reset after use
            verify_token(grand, tokens, start, input);
        }
    }
    tokens[grand->chicken.token_count++] = (t_Token){TK_END, NULL};
    return (tokens);
}

// t_Token *lexer(const char *input, t_grand *grand)
// {
//     t_Token *tokens = malloc(sizeof(t_Token) * 100);
//     int i;

//     if (!tokens)
//         return NULL;
//     i = 0;
//     while(input[i])
//     {
//         while(isspace(input[i]))
//             i++;
//         if(input[i] == '|')
//             tokens[grand->chicken.token_count++] = (t_Token){TOKEN_PIPE, ft_strdup("|")};
//         else if(input[i] == '>')
//             tokens[grand->chicken.token_count++] = (t_Token){TOKEN_REDIRECT_OUT, ft_strdup(">")};
//         else if(input[i] == '<')
//             tokens[grand->chicken.token_count++] = (t_Token){TOKEN_REDIRECT_IN, ft_strdup("<")};
//         else if(input[i] == '>' && input[i + 1] == '>')
//         {
//             tokens[grand->chicken.token_count++] = (t_Token){TK_AOUT, ft_strdup(">>")};
//             i++; // Skip the second '>'
//         }
//         else if(input[i] == '-') // cmd options
//         {
//             int start = i;
//             while (input[i] && !isspace(input[i]) && !ft_strchr("|<>", input[i]))
//                 i++;
//             tokens[grand->chicken.token_count++] = (t_Token){TOKEN_OPTION, strndup(&input[start], i - start)};
//         }
//         else // command
//         {
//             int start = i;
//             while (input[i] && !isspace(input[i]) && !ft_strchr("|<>", input[i]))
//                 i++;
//             tokens[grand->chicken.token_count++] = (t_Token){TOKEN_COMMAND, strndup(&input[start], i - start)};
//         }
//         if (input[i])
//             i++;
//     }
//     tokens[grand->chicken.token_count++] = (t_Token){TOKEN_END, NULL};
//     printf("tokens %s %s %s enum: %d %d\n", tokens[0].value, tokens[1].value, tokens[2].value, (int)tokens[0].type, (int)tokens[1].type);
//     return (tokens);
// }
