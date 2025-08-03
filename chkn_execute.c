#include "chicken.h"

void should_redirect(t_ASTNode *node)
{
    // printf("Redirecting: std_in = %d, std_out = %d %s, %s, %s, %s\n", node->std_in, node->std_out, node->args[0], node->args[1], node->args[2], node->args[3], node->args[4]);
    if (node->type != NODE_BUILTIN)
        {
            if (node->std_in != 0)
                dup2(node->std_in, STDIN_FILENO);
        }
    if (node->std_out != 1)
        dup2(node->std_out, STDOUT_FILENO);
}

void call_builtin(t_ASTNode *node, t_grand *grand)
{
    int status;

    status = 0;
    should_redirect(node);
     printf("Executing built-in command: %s\n", node->args[0]);
    if (!ft_strncmp(node->args[0], "cd", 2))
        status = chkn_cd(node->args, &grand->env.envp);
    else if (!ft_strncmp(node->args[0], "echo", 4))
        status = chkn_echo(node->args);
    else if (!ft_strncmp(node->args[0], "env", 3))
        status = chkn_prnt_envp(node->args, grand->env.envp);
    else if (!ft_strncmp(node->args[0], "exit", 4))
        status = chkn_exit(node->args);
    else if (!ft_strncmp(node->args[0], "export", 6))
        status = chkn_export(&grand->env.envp, node->args);
    else if (!ft_strncmp(node->args[0], "pwd", 3))
        status = chkn_pwd(node->args, grand->env.envp);
    else if (!ft_strncmp(node->args[0], "unset", 3))
        status = chkn_unset(&grand->env.envp, node->args);
    if (node->last_cmd == 1)
                grand->chicken.status = status;
}

void execute_blt(t_ASTNode *node, t_grand *grand)
{
    if (grand->pflag == 1)
    {
        {
            call_builtin(node, grand);
            printf("Built-in command executed with status: %d\n", grand->chicken.status);
            exit(grand->chicken.status);
        }
    }
    else
    {
        call_builtin(node, grand);
        printf("Built-in command executed with status: %d\n", grand->chicken.status);
        return ;
    }
}

// call cleanup if exec fails
void execute_command(t_ASTNode *node, t_grand *grand)
{
     if (node->type == NODE_COMMAND || node->type == NODE_BUILTIN)
    {

        printf("Executing command: %s %s\n", node->args[0], node->args[1] ? node->args[1] : "");
        if (node->type == NODE_BUILTIN && grand->pflag == 0)
        {
            call_builtin(node, grand);
            printf("Built-in command executed with status: %d\n", grand->chicken.status);
            return ;
        }
        if (fork() == 0)
        {
            if(node->type == NODE_BUILTIN)
                execute_blt(node, grand);
            printf("in %d out %d\n\n\n\n", node->std_in, node->std_out);
            printf("this is %s\n", node->args[0]);
            should_redirect(node);
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_IGN);
			get_path(grand, node->args[0]);
			printf("execve path %s\n", grand->env.full_path);
            printf("%s\n", node->args[0]);
            execve(grand->env.full_path, node->args, grand->env.envp);
            // execvp(node->args[0], node->args);
            if (node->last_cmd == 1)
                grand->chicken.status = errno;
            printf("last cmd %d\n", node->last_cmd);
            ft_printf("status %d\n", grand->chicken.status);
            perror("");
            free(grand->env.full_path);
            close_redirection_fds();
            exit(grand->chicken.status);
        }
        signal(SIGINT, SIG_IGN);
        wait(&grand->chicken.status);
        if (WIFEXITED(grand->chicken.status))
            grand->chicken.status = WEXITSTATUS(grand->chicken.status);
        else if (WIFSIGNALED(grand->chicken.status))
            grand->chicken.status = 128 + WTERMSIG(grand->chicken.status);
        // grand->chicken.status = WEXITSTATUS(grand->chicken.status);
        printf("Command executed with status: %d\n", grand->chicken.status);
        signal(SIGINT, sigint_handler);
    }
}

void close_wait(int fd[2], t_grand *grand)
{
    close(fd[0]);
    close(fd[1]);
    signal(SIGINT, SIG_IGN);
    wait(&grand->chicken.status);
    printf("1status %d\n", WEXITSTATUS(grand->chicken.status));
    wait(&grand->chicken.status);
    printf("2status %d\n", WEXITSTATUS(grand->chicken.status));
    dup2(grand->saved_stdin, STDIN_FILENO);
    dup2(grand->saved_stdout, STDOUT_FILENO);
    signal(SIGINT, sigint_handler);
}

void fork_logic(t_ASTNode *node, t_grand *grand, int fd[2], int flag)
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

void execute(t_ASTNode *node, t_grand *grand)
{
    int fd[2];

    if (!node)
        return;
    execute_command(node, grand);
    if (node->type == NODE_PIPE)
    {
        grand->pflag = 1;
        pipe(fd);
        if (fork() == 0)
        {
            fork_logic(node->left, grand, fd, 0);
        }
        if (fork() == 0)
        {
            fork_logic(node->right, grand, fd, 1);
        }
        close_wait(fd, grand);
        grand->chicken.status = WEXITSTATUS(grand->chicken.status);
    }
}
