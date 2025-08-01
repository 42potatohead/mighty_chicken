/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 19:17:06 by ataan             #+#    #+#             */
/*   Updated: 2024/09/06 19:17:07 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*The strnstr() function locates the first	occurrence of the  null-termi-
		nated  string little in the string big, where not more than len charac-
		ters are	searched.  Characters that appear after	a `\0'	character  are
		not  searched.  Since the strnstr() function is a FreeBSD specific API,
		it should only be used when portability is not a	concern.*/
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	llen;
	size_t	blen;

	llen = ft_strlen(little);
	blen = ft_strlen(big);
	if (llen == 0)
		return ((char *)big);
	else if (blen == 0)
		return (NULL);
	if (llen > len)
		return (NULL);
	i = 0;
	while (i <= len - llen)
	{
		if (ft_strncmp(&big[i], little, llen) == 0)
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
