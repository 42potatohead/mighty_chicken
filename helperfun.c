#include "chicken.h"

char	*ft_strndup(const char *s, int len)
{
	char	*ns;
	size_t	len;
	size_t	i;

	ns = (char *)malloc((len + 1) * sizeof(char));
	if (ns == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ns[i] = s[i];
		i++;
	}
	ns[i] = '\0';
	return (ns);
}
