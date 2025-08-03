/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 13:06:23 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/08/03 21:26:38 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

t_ASTNode	*create_node(t_grand *grand, t_ASTNode *left, t_ASTNode *right)
{
	t_ASTNode	*node;

	node = malloc(sizeof(t_ASTNode));
	node->type = grand->astatrr->type;
	node->args = grand->astatrr->args;
	node->left = left;
	node->right = right;
	node->last_cmd = grand->astatrr->last_cmd;
	node->std_in = grand->astatrr->std_in;
	node->std_out = grand->astatrr->std_out;
	free(grand->astatrr);
	grand->astatrr = malloc(sizeof(t_ASTatrr));
	return (node);
}

int	parse_in(t_Token **tokens, t_grand *grand)
{
	int	std_in;
	int	std_out;

	std_in = 0;
	if ((*tokens)->type == TK_R_IN)
	{
		std_in = handle_redirect(*tokens, grand);
		if (std_in == -1)
			return (-1);
		(*tokens) += 2;
		return (std_in);
	}
	std_out = 1;
	if ((*tokens)->type == TK_R_OUT)
	{
		std_out = handle_redirect(*tokens, grand);
		if (std_out == -1)
			return (EXIT_FAILURE);
		(*tokens) += 2;
		return (std_out);
	}
	return (EXIT_FAILURE);
}

void	determine_redirect(t_Token **tokens, t_ASTNode *node, t_grand *grand)
{
	if ((*tokens)->type == TK_R_IN)
	{
		node->std_in = handle_redirect(*tokens, grand);
		if (node->std_in == -1)
			return ;
	}
	if ((*tokens)->type == TK_R_OUT)
	{
		node->std_out = handle_redirect(*tokens, grand);
		if (node->std_out == -1)
			return ;
	}
}

void	fill_pipe(t_Token **tokens, t_ASTNode *right, t_grand *grand)
{
	determine_redirect(tokens, right, grand);
	grand->astatrr->type = NODE_PIPE;
	grand->astatrr->args = malloc(sizeof(char *) * 2);
	grand->astatrr->args[0] = ft_strdup("|");
	grand->astatrr->args[1] = NULL;
	grand->astatrr->last_cmd = 0;
	grand->astatrr->std_in = right->std_in;
	grand->astatrr->std_out = right->std_out;
}

t_ASTNode	*parse_expression(t_Token **tokens, t_grand *grand)
{
	t_ASTNode	*left;
	t_NodeType	type;
	t_ASTNode	*right;

	if (!tokens || !(*tokens))
		return (NULL);
	left = NULL;
	if ((*tokens)->type == TK_CMD || (*tokens)->type == TK_BLTN
		|| (*tokens)->type == TK_R_IN || (*tokens)->type == TK_R_OUT)
		left = parse_command(tokens, grand);
	if ((*tokens)->type == TK_END)
		return (left);
	while ((*tokens)->type == TK_PIPE)
	{
		if ((*tokens)->type == TK_PIPE)
			type = NODE_PIPE;
		(*tokens)++;
		right = parse_command(tokens, grand);
		fill_pipe(tokens, right, grand);
		left = create_node(grand, left, right);
	}
	return (left);
}
