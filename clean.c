/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:52:52 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:28:43 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

void	free_ast(t_ASTNode *ast)
{
	int	i;

	i = 0;
	if (!ast)
		return ;
	if (ast->left)
		free_ast(ast->left);
	if (ast->right)
		free_ast(ast->right);
	if (ast->args)
	{
		while (ast->args[i])
		{
			free(ast->args[i]);
			i++;
		}
		free(ast->args);
	}
	free(ast);
}

void	free_tokens(t_Token *tokens, int count)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < count && tokens[i].type != TK_END)
	{
		if (tokens[i].value)
			free(tokens[i].value);
		i++;
	}
	if (i < count && tokens[i].type == TK_END && tokens[i].value)
		free(tokens[i].value);
	free(tokens);
}

void	free_env(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void	close_redirection_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void	clean_exit(t_grand *grand, t_Token *tokens, t_ASTNode *ast)
{
	dup2(grand->saved_stdin, 0);
	dup2(grand->saved_stdout, 1);
	if (tokens)
		free_tokens(tokens, grand->chicken.token_count);
	if (ast)
		free_ast(ast);
	close_redirection_fds();
	free(grand->chicken.input);
}
