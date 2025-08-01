/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:27:42 by ataan             #+#    #+#             */
/*   Updated: 2024/12/10 16:35:13 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	cleanup_fd(char **line_draft, int fd)
{
	if (line_draft[fd])
	{
		free(line_draft[fd]);
		line_draft[fd] = NULL;
	}
}

static char	*extract_line(char **line_draft, int fd)
{
	char	*line;
	int		nl;

	if (!line_draft[fd])
		return (NULL);
	nl = 0;
	while (line_draft[fd][nl] != '\n' && line_draft[fd][nl] != '\0')
		nl++;
	if (nl == 0 && line_draft[fd][0] != '\n')
	{
		if (gnl_strlen(line_draft[fd]) == 0)
			line = NULL;
		else
			line = ft_strdup(line_draft[fd]);
		cleanup_fd(line_draft, fd);
		return (line);
	}
	line = ft_substr(line_draft[fd], 0, nl + 1);
	if (line == NULL)
	{
		cleanup_fd(line_draft, fd);
		return (NULL);
	}
	return (line);
}

static char	*update_line_draft(char **line_draft, int fd, char *line)
{
	char	*tmp;
	int		line_len;

	tmp = line_draft[fd];
	line_len = 0;
	if (line != NULL)
		line_len = gnl_strlen(line);
	line_draft[fd] = ft_substr(tmp, line_len, (gnl_strlen(line_draft[fd])
				- line_len));
	free(tmp);
	return (line_draft[fd]);
}

static char	*read_line(char *buff, char **line_draft, int fd)
{
	int	bytes_read;

	buff = (char *)malloc(BUFFER_SIZE + 1);
	if (buff == NULL)
	{
		cleanup_fd(line_draft, fd);
		return (NULL);
	}
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(line_draft[fd], '\n'))
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buff);
			cleanup_fd(line_draft, fd);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buff[bytes_read] = '\0';
		line_draft[fd] = gnl_strjoin(line_draft[fd], buff);
	}
	free(buff);
	return ("OK");
}

char	*get_next_line(int fd)
{
	char		*buff;
	static char	*line_draft[1024];
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (line_draft[fd] == NULL)
		line_draft[fd] = ft_strdup("");
	buff = NULL;
	if (read_line(buff, line_draft, fd))
	{
		line = extract_line(line_draft, fd);
		line_draft[fd] = update_line_draft(line_draft, fd, line);
	}
	return (line);
}
