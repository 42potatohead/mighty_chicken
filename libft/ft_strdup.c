/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 19:18:48 by ataan             #+#    #+#             */
/*   Updated: 2024/09/06 19:18:50 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*The strdup() function returns a pointer to a new string which is a duplicate
 of the string s. Memory for the new string is obtained with malloc(3),
 and can be freed with free(3)*/
char	*ft_strdup(const char *s)
{
	char	*ns;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
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
