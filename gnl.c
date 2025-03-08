#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif

char *gnl(int fd)
{
	static char buffer[BUFFER_SIZE];
	static int bytes_read = 0, pos = 0;
	int i = 0;
	char line[900000];

	if(fd <= 0 || BUFFER_SIZE <= 0)
		return NULL;
	while(1)
	{
		if(pos >= bytes_read)
		{
			bytes_read = read(fd, buffer, BUFFER_SIZE);
			pos = 0;
			if(bytes_read <= 0)
				break;
		}
		line[i++] = buffer[pos++];
		if(buffer[pos] == '\n')
			break;
	}

	if (i == 0)
		return NULL;

	char *result = malloc(sizeof(char) * i);
	for(int x = 0; x < i; x++)
			result[x] = line[x];
	return (result);
}

int main(void)
{
	int fd;
	fd = open("file.txt", O_RDONLY);

	printf("%s", gnl(fd));
	printf("%s", gnl(fd));
	printf("%s", gnl(fd));
	printf("%s", gnl(fd));
	printf("%s", gnl(fd));
	printf("%s", gnl(fd));
	printf("%s", gnl(fd));
	printf("%s", gnl(fd));
	printf("%s", gnl(fd));


}
