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
t_ASTNode *create_node(e_NodeType type, char **args, t_ASTNode *left, t_ASTNode *right, int last_cmd)
{
    t_ASTNode *node = malloc(sizeof(t_ASTNode));
    node->type = type;
    node->args = args;
    node->left = left;
    node->right = right;
    node->last_cmd = last_cmd;
    return (node);
}

/*
    Create the AST to represent the structure of complex shell commands
    right now it does commands
*/
t_ASTNode *parse_command(t_Token **tokens)
{
    int last_cmd = 0;
    char **args = malloc(sizeof(char *) * 10);
    int arg_count = 0;
    int flag = 0;
    e_NodeType type;

    int i = 1;
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

    if(flag == 1)
        type = NODE_BUILTIN;
    return (create_node(type, args, NULL, NULL, last_cmd));
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

t_ASTNode *parse_expression(t_Token **tokens)
{
    if (!tokens || !(*tokens))
    {
        perror("No tokens to parse\n");
        return NULL; // Handle the case where there are no tokens
    }
    t_ASTNode *left = malloc(sizeof(t_ASTNode));

    if ((*tokens)->type == TOKEN_COMMAND || (*tokens)->type == TOKEN_BUILTIN)
        left = parse_command(tokens);
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
        t_ASTNode *right = parse_command(tokens);
        left = create_node(type, args, left, right, 0);
    }
    return (left);
}
