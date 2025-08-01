/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 19:18:25 by ataan             #+#    #+#             */
/*   Updated: 2024/09/06 19:18:27 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*The strlcat() function appends the NUL-terminated string src to the end
 of dst. It will append at most size - strlen(dst) - 1 bytes,
 NUL-terminating the result.*/
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	i;
	size_t	res;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	if (size > len_dst)
		res = len_src + len_dst;
	else
		res = len_src + size;
	if (size == 0)
		return (res);
	i = 0;
	while (src[i] && (len_dst < size - 1))
	{
		dst[len_dst] = src[i];
		len_dst++;
		i++;
	}
	if (len_dst < size)
		dst[len_dst] = '\0';
	return (res);
}
