/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chkn_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:27:57 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:28:27 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

void	call_builtin(t_ASTNode *node, t_grand *grand)
{
	int	status;

	status = 0;
	should_redirect(node);
	if (!ft_strncmp(node->args[0], "cd", 2))
		status = chkn_cd(node->args, &grand->env.envp);
	else if (!ft_strncmp(node->args[0], "echo", 4))
		status = chkn_echo(node->args);
	else if (!ft_strncmp(node->args[0], "env", 3))
		status = chkn_prnt_envp(node->args, grand->env.envp);
	else if (!ft_strncmp(node->args[0], "exit", 4))
		status = chkn_exit(node->args, grand);
	else if (!ft_strncmp(node->args[0], "export", 6))
		status = chkn_export(&grand->env.envp, node->args);
	else if (!ft_strncmp(node->args[0], "pwd", 3))
		status = chkn_pwd(node->args, grand->env.envp);
	else if (!ft_strncmp(node->args[0], "unset", 3))
		status = chkn_unset(&grand->env.envp, node->args);
	if (node->last_cmd == 1)
		grand->chicken.status = status;
}

void	execute_blt(t_ASTNode *node, t_grand *grand)
{
	if (grand->pflag == 1)
	{
		{
			call_builtin(node, grand);
			exit(grand->chicken.status);
		}
	}
	else
	{
		call_builtin(node, grand);
		return ;
	}
}

void	execute_command(t_ASTNode *node, t_grand *grand)
{
	if (node->type == NODE_COMMAND || node->type == NODE_BUILTIN)
	{
		if (node->type == NODE_BUILTIN && grand->pflag == 0)
		{
			call_builtin(node, grand);
			return ;
		}
		if (fork() == 0)
		{
			should_redirect(node);
			if (node->type == NODE_BUILTIN)
				execute_blt(node, grand);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_IGN);
			get_path(grand, node->args[0]);
			execve(grand->env.full_path, node->args, grand->env.envp);
			if (node->last_cmd == 1)
				grand->chicken.status = errno;
			perror("chicken");
			bath_time(grand);
			exit(grand->chicken.status);
		}
		wait_child(grand);
	}
}

void	fork_logic(t_ASTNode *node, t_grand *grand, int fd[2], int flag)
{
	signal(SIGINT, SIG_DFL);
	if (flag == 0)
		dup2(fd[1], STDOUT_FILENO);
	else
		dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	execute(node, grand);
	dup2(grand->saved_stdin, STDIN_FILENO);
	dup2(grand->saved_stdout, STDOUT_FILENO);
	exit(grand->chicken.status);
}

void	execute(t_ASTNode *node, t_grand *grand)
{
	int		fd[2];
	pid_t	last_pid;

	if (!node)
		return ;
	grand->original_ast = node;
	execute_command(node, grand);
	if (node->type == NODE_PIPE)
	{
		grand->pflag = 1;
		pipe(fd);
		if (fork() == 0)
		{
			fork_logic(node->left, grand, fd, 0);
		}
		last_pid = fork();
		if (last_pid == 0)
		{
			fork_logic(node->right, grand, fd, 1);
		}
		close_wait(fd, grand, last_pid);
		grand->chicken.status = WEXITSTATUS(grand->chicken.status);
	}
}
