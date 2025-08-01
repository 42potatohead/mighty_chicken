/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 19:23:59 by ataan             #+#    #+#             */
/*   Updated: 2024/09/06 19:24:00 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*The calloc() function allocates memory for an array of nmemb elements of size
 bytes each and returns a pointer to the allocated memory. The memory is set to
  zero. If nmemb or size is 0, then calloc() returns either NULL, or a unique
  pointer value that can later be successfully passed to free(). */
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;
	size_t	total_size;

	total_size = nmemb * size;
	if (nmemb != 0 && size != 0 && total_size / nmemb != size)
		return (NULL);
	arr = malloc(total_size);
	if (arr != NULL)
		ft_bzero(arr, total_size);
	return (arr);
}
