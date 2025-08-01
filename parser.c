#include "chicken.h"

/*
    Creates nodes for the AST (Abstract Syntax Tree)
    left and right can be used for more complex command structures such as pipes
    eg
                        NODE_PIPE
                        /        \
                NODE_COMMAND   NODE_COMMAND
                ("ls", "-la")  ("grep", "main")
                    /                 \
            redirect out          redirect in
    NODE_PIPE acts as the "parent" — its left child is the ls command and its right child is the grep command.
*/
t_ASTNode *create_node(e_NodeType type, char **args, t_ASTNode *left, t_ASTNode *right, int last_cmd, int std_in, int std_out)
{
    t_ASTNode *node = malloc(sizeof(t_ASTNode));
    node->type = type;
    node->args = args;
    node->left = left;
    node->right = right;
    node->last_cmd = last_cmd;
    node->std_in = std_in;
    node->std_out = std_out;
    return (node);
}

int parse_in(t_Token **tokens, t_grand *grand)
{
    int std_in;
    int std_out;

    std_in = 0;
    if((*tokens)->type == TOKEN_REDIRECT_IN)
    {
        std_in = handle_redirect(*tokens, grand);
        if(std_in == -1)
            return (-1);
        (*tokens) += 2; // Skip the redirect token and the file name
        printf("we are on token %s\n", (*tokens)->value);
        return (std_in);
    }
    std_out = 1;
    if((*tokens)->type == TOKEN_REDIRECT_OUT)
    {
        std_out = handle_redirect(*tokens, grand);
        if(std_out == -1)
            return (NULL);
        (*tokens) += 2;
        return (std_out);
    }
    return (EXIT_FAILURE);
}

/*
    Create the AST to represent the structure of complex shell commands
    right now it does commands
*/
t_ASTNode *parse_command(t_Token **tokens, t_grand *grand)
{
    int last_cmd = 0;
    char **args = malloc(sizeof(char *) * 100);
    int arg_count = 0;
    int flag = 0;
    e_NodeType type;
    int std_in = 0;
    int std_out = 1;

    if((*tokens)->type == TOKEN_HEREDOC)
    {
        handle_redirect(*tokens, grand);
        (*tokens)++;
    }
    if((*tokens)->type == TOKEN_REDIRECT_IN)
        std_in = parse_in(tokens, grand);
    printf("STD IN = %d\n", std_in);
    while ((*tokens)->type == TOKEN_COMMAND || (*tokens)->type == TOKEN_BUILTIN)
    {
        printf("testing : %s\n", (*tokens)->value);
        args[arg_count++] = ft_strdup((*tokens)->value);
        printf("token type %d\n", (*tokens)->type);
        if ((*tokens)->type == TOKEN_COMMAND)
            type = NODE_COMMAND;
        if ((*tokens)->type == TOKEN_BUILTIN)
        {
            flag = 1;
            type = NODE_BUILTIN;
        }
        (*tokens)++;
        if ((*tokens)->type == TOKEN_END)
            last_cmd = 1;
        printf("node type %d\n", type);
    }
    args[arg_count] = NULL;
    if((*tokens)->type == TOKEN_REDIRECT_IN)
        std_in = parse_in(tokens, grand);
    if((*tokens)->type == TOKEN_HEREDOC)
    {
        handle_redirect(*tokens, grand);
        (*tokens)++;
    }
    if((*tokens)->type == TOKEN_REDIRECT_OUT)
        std_out = parse_in(tokens, grand);

    if(flag == 1)
        type = NODE_BUILTIN;
    return (create_node(type, args, NULL, NULL, last_cmd, std_in, std_out));
}

// t_ASTNode *parse_builtin(t_Token **tokens)
// {
// 	char **args = malloc(sizeof(char *) * 10);
// 	int arg_count = 0;

// 	while ((*tokens)->type == TOKEN_BUILTIN)
// 	{
// 		args[arg_count++] = ft_strdup((*tokens)->value);
// 		(*tokens)++;
// 		if ((*tokens)->type == TOKEN_END)
// 			break;
// 	}
// 	args[arg_count] = NULL;

// 	return (create_node(NODE_BUILTIN, args, NULL, NULL, 0));
// }

void determine_redirect(t_Token **tokens, t_ASTNode *node, t_grand *grand)
{
    if((*tokens)->type == TOKEN_REDIRECT_IN)
    {
        node->std_in = handle_redirect(*tokens, grand);
        if(node->std_in == -1)
            return ;
    }
    if((*tokens)->type == TOKEN_REDIRECT_OUT)
    {
        node->std_out = handle_redirect(*tokens, grand);
        if(node->std_out == -1)
            return ;
    }
    // (*tokens) += 2;
}

t_ASTNode *parse_expression(t_Token **tokens, t_grand *grand)
{
    if (!tokens || !(*tokens))
    {
        perror("No tokens to parse\n");
        return NULL; // Handle the case where there are no tokens
    }
    t_ASTNode *left = NULL;


    if ((*tokens)->type == TOKEN_COMMAND || (*tokens)->type == TOKEN_BUILTIN || (*tokens)->type == TOKEN_REDIRECT_IN || (*tokens)->type == TOKEN_REDIRECT_OUT)
        left = parse_command(tokens, grand);
    if ((*tokens)->type == TOKEN_END)
        return left; // If we reach the end, return the left node
    printf("end\n");
    char **args = malloc(sizeof(char *) * 50);
    args[0] = ft_strdup((*tokens)->value);
    args[1] = NULL;
    while ((*tokens)->type == TOKEN_PIPE)
    {
        e_NodeType type;
        if ((*tokens)->type == TOKEN_PIPE)
            type = NODE_PIPE;
        (*tokens)++;
        t_ASTNode *right = parse_command(tokens, grand);
        determine_redirect(tokens, right, grand);
        left = create_node(type, args, left, right, 0, right->std_in, right->std_out);
    }
    return (left);
}
