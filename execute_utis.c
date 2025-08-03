/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 21:13:32 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:23:51 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

void	close_wait(int fd[2], t_grand *grand, pid_t last_pid)
{
	int	status;

	close(fd[0]);
	close(fd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		grand->chicken.status = status;
	waitpid(-1, &status, 0);
	dup2(grand->saved_stdin, STDIN_FILENO);
	dup2(grand->saved_stdout, STDOUT_FILENO);
	signal(SIGINT, sigint_handler);
}

void	should_redirect(t_ASTNode *node)
{
	if (node->type != NODE_BUILTIN)
	{
		if (node->std_in != 0)
			dup2(node->std_in, STDIN_FILENO);
	}
	if (node->std_out != 1)
		dup2(node->std_out, STDOUT_FILENO);
}

void	wait_child(t_grand *grand)
{
	signal(SIGINT, SIG_IGN);
	wait(&grand->chicken.status);
	if (WIFEXITED(grand->chicken.status))
		grand->chicken.status = WEXITSTATUS(grand->chicken.status);
	else if (WIFSIGNALED(grand->chicken.status))
		grand->chicken.status = 128 + WTERMSIG(grand->chicken.status);
	signal(SIGINT, sigint_handler);
}

void	bath_time(t_grand *grand)
{
	free(grand->env.full_path);
	close_redirection_fds();
}
