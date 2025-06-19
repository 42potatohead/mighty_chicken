#include "chicken.h"

void call_builtin(t_ASTNode *node, t_grand *grand)
{
     printf("Executing built-in command: %s\n", node->args[0]);
    if (!ft_strncmp(node->args[0], "cd", 2))
        chkn_cd(node->args, &grand->env.envp);
    else if (!ft_strncmp(node->args[0], "echo", 4))
        chkn_echo(node->args);
    else if (!ft_strncmp(node->args[0], "env", 3))
           chkn_prnt_envp(node->args, grand->env.envp);
    else if (!ft_strncmp(node->args[0], "exit", 4))
        chkn_exit(node->args);
    else if (!ft_strncmp(node->args[0], "export", 6))
        chkn_export(&grand->env.envp, node->args);
    else if (!ft_strncmp(node->args[0], "pwd", 3))
        chkn_pwd(node->args);
    else if (!ft_strncmp(node->args[0], "pwd", 3))
        chkn_unset(&grand->env.envp, node->args);
}
// call cleanup if exec fails
void execute_command(t_ASTNode *node, t_grand *grand)
{
     if (node->type == NODE_COMMAND || node->type == NODE_BUILTIN)
    {
        printf("Executing command: %s %s\n", node->args[0], node->args[1] ? node->args[1] : "");
        // If not a built-in command, execute it as an external command
        if(node->type == NODE_BUILTIN)
            call_builtin(node, grand);
        else if (fork() == 0)
        {
			// get_path(grand, node->args[0]);
			printf("execve path %s\n", grand->env.full_path);
            printf("%s\n", node->args[0]);
            // execve(grand->env.full_path, node->args, grand->env.envp);
            execvp(node->args[0], node->args);
            if (node->last_cmd == 1)
                grand->chicken.status = errno;
            ft_printf("status %d\n", grand->chicken.status);
            perror("execvp");
            exit(grand->chicken.status);
        }
        wait(&grand->chicken.status);
        grand->chicken.status = WEXITSTATUS(grand->chicken.status);
    }
}

void close_wait(int fd[2], t_grand *grand)
{
    close(fd[0]);
    close(fd[1]);
    wait(&grand->chicken.status);
    wait(&grand->chicken.status);
}

void execute(t_ASTNode *node, t_grand *grand)
{
    if (!node)
    	return;

    execute_command(node, grand);
    if (node->type == NODE_PIPE)
    {
        int fd[2];
        pipe(fd);
        if (fork() == 0)
        {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execute(node->left, grand);
            exit(grand->chicken.status);
        }
        if (fork() == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execute(node->right, grand);
            exit(grand->chicken.status);
        }
        close_wait(fd, grand);
    }
}
