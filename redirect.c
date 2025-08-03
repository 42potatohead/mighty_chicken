#include "chicken.h"

int redirect_in(char *file, t_grand *grand)
{
	grand->saved_stdin = dup(STDIN_FILENO);
	printf("file = %s\n", file);
	int fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		grand->chicken.status = errno;
		printf("status number == %d\n\n\n\n", grand->chicken.status);
		perror("");
		return (0);
	}
	// grand.chicken.redirectin = 1;
	return (fd);
}
int redirect_out(char *file, t_grand *grand, int append_flag)
{
	grand->saved_stdout = dup(STDOUT_FILENO);
	printf("file = %s\n", file);
	int fd;

	if (append_flag == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		grand->chicken.status = errno;
		printf("status number == %d\n\n\n\n", grand->chicken.status);
		perror("");
		return (0);
	}
	return (fd);

}

int redirect_heredoc(char *delimiter, t_grand *grand)
{
    int pipefd[2];
    char *line = NULL;

    if (pipe(pipefd) == -1)
        return (-1);

	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
			break;
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipefd[1]);
	return (pipefd[0]); // Return the read end as the fd for std_in
}

int handle_redirect(t_Token *tokens, t_grand *grand)
{
	char *file;
	int fd;
	int append_flag;

	append_flag = 0;
	printf("we are there\n");
	if (!ft_strncmp(tokens->value, "<<", 2))
	{
		tokens++;
		fd = redirect_heredoc(tokens->value, grand);
		return (fd);
	}
	if (!ft_strncmp(tokens->value, ">", 1) || !ft_strncmp(tokens->value, ">>", 2))
	{
		if (!ft_strncmp(tokens->value, ">>", 2))
			append_flag = 1;
		else
			append_flag = 0;
		tokens++;
		fd = redirect_out(tokens->value, grand, append_flag);
		return (fd);
	}
	if (!ft_strncmp(tokens->value, "<", 1))
	{
		tokens++;
		fd = redirect_in(tokens->value, grand);
		return (fd);
	}
	return (0);
}
