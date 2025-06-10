#include "chicken.h"


int count_tokens(char *input)
{
    int count = 0;
    int i = 0;
    while(input[i])
    {
        while(isspace(input[i]))
            input++;
        if(input[i] == '|' || input[i] == '>' || input[i] == '<' || (input[i] == '>' && input[i + 1] == '>'))
            count++;
        else
        {
            const char *start = input;
            while (*input && !isspace(input[i]) && !ft_strchr("|<>", input[i]))
                i++;
            count++;
        }
        if (input[i])
            i++; // fix garbage
    }
    printf("count %d\n", count);
    return (count);
}

int catagory(const char *value)
{
    if (!ft_strncmp(value, "cd", 2) || !ft_strncmp(value, "echo", 4) || !ft_strncmp(value, "env", 3) ||
        !ft_strncmp(value, "exit", 4) || !ft_strncmp(value, "export", 6) || !ft_strncmp(value, "unset", 5) ||
        !ft_strncmp(value, "pwd", 3))
        return (TOKEN_BUILTIN);
    else if (!ft_strncmp(value, "|", 8))
        return (TOKEN_PIPE);
    else if (!ft_strncmp(value, ">", 1))
        return (TOKEN_REDIRECT_OUT);
    else if (!ft_strncmp(value, "<", 1))
        return (TOKEN_REDIRECT_IN);
    else if (!ft_strncmp(value, ">>", 2))
        return (TOKEN_APPENOUT);
    else
        return (TOKEN_COMMAND);
}

t_Token create_token(e_TokenType type, const char *value, int len)
{
    t_Token token;

    token.value = strndup(value, len);
    token.type = catagory(token.value);
    if (!token.value)
    {
        perror("Failed to allocate memory for token value");
        exit(EXIT_FAILURE);
    }
    printf("token value %s type %d\n", token.value, (int)token.type);
    return (token);
}

/*
    lexer, it converts text into meaningful lexical tokens, defined by the Token enum struct
*/
t_Token *lexer(const char *input, t_grand *grand)
{
    t_Token *tokens = malloc(sizeof(t_Token) * (count_tokens((char *)input) + 1));

    while(*input)
    {
        while(isspace(*input))
            input++;
        if(*input == '|')
            tokens[grand->chicken.token_count++] = create_token(TOKEN_PIPE, "|", 1);
        else if(*input == '>')
            tokens[grand->chicken.token_count++] = create_token(TOKEN_REDIRECT_OUT, ">", 1);
        else if(*input == '<')
            tokens[grand->chicken.token_count++] = create_token(TOKEN_REDIRECT_IN, "<", 1);
        else if(*input == '>' && *(input + 1) == '>')
            tokens[grand->chicken.token_count++] = create_token(TOKEN_APPENOUT, ">>", 2);
        // else if(*input == '-') // cmd options
        // {
        //     const char *start = input;
        //     while (*input && !isspace(*input) && !ft_strchr("|<>", *input))
        //         input++;
        //     tokens[grand->chicken.token_count++] = (t_Token){TOKEN_OPTION, strndup(start, input - start)};
        // }
        else // command
        {
            const char *start = input;
            while (*input && !isspace(*input) && !ft_strchr("|<>", *input))
                input++;
            tokens[grand->chicken.token_count++] = create_token(TOKEN_COMMAND, start, input - start);
        }
        if (*input)
            input++; // fix garbage
    }
    tokens[grand->chicken.token_count++] = (t_Token){TOKEN_END, NULL};
    // printf("tokens %s %s %s enum: %d %d\n", tokens[0].value, tokens[1].value, tokens[2].value, (int)tokens[0].type, (int)tokens[1].type);
    return (tokens);
}

// with I
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
//             tokens[grand->chicken.token_count++] = (t_Token){TOKEN_APPENOUT, ft_strdup(">>")};
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
