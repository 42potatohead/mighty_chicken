/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parse_logic.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:21:39 by zabu-bak          #+#    #+#             */
/*   Updated: 2025/08/03 21:25:11 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

void	init_command_parsing(t_grand *grand)
{
	grand->astatrr->args = malloc(sizeof(char *) * 100);
	grand->astatrr->last_cmd = 0;
	grand->astatrr->std_in = 0;
	grand->astatrr->std_out = 1;
}

void	handle_pre_command_redirects(t_Token **tokens, t_grand *grand)
{
	if ((*tokens)->type == TK_R_DILM)
	{
		handle_redirect(*tokens, grand);
		(*tokens)++;
	}
	if ((*tokens)->type == TK_R_IN)
		grand->astatrr->std_in = parse_in(tokens, grand);
}

int	parse_command_arguments(t_Token **tokens, t_grand *grand)
{
	int	arg_count;
	int	flag;

	arg_count = 0;
	flag = 0;
	while ((*tokens)->type == TK_CMD || (*tokens)->type == TK_BLTN)
	{
		grand->astatrr->args[arg_count++] = ft_strdup((*tokens)->value);
		if ((*tokens)->type == TK_CMD)
			grand->astatrr->type = NODE_COMMAND;
		if ((*tokens)->type == TK_BLTN)
		{
			flag = 1;
			grand->astatrr->type = NODE_BUILTIN;
		}
		(*tokens)++;
		if ((*tokens)->type == TK_END)
			grand->astatrr->last_cmd = 1;
	}
	grand->astatrr->args[arg_count] = NULL;
	return (flag);
}

void	handle_post_command_redirects(t_Token **tokens, t_grand *grand)
{
	if ((*tokens)->type == TK_R_IN)
		grand->astatrr->std_in = parse_in(tokens, grand);
	if ((*tokens)->type == TK_R_DILM)
	{
		handle_redirect(*tokens, grand);
		(*tokens)++;
	}
	if ((*tokens)->type == TK_R_OUT)
		grand->astatrr->std_out = parse_in(tokens, grand);
}

t_ASTNode	*parse_command(t_Token **tokens, t_grand *grand)
{
	int	flag;

	init_command_parsing(grand);
	handle_pre_command_redirects(tokens, grand);
	flag = parse_command_arguments(tokens, grand);
	handle_post_command_redirects(tokens, grand);
	if (flag == 1)
		grand->astatrr->type = NODE_BUILTIN;
	return (create_node(grand, NULL, NULL));
}
