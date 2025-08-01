/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 19:19:17 by ataan             #+#    #+#             */
/*   Updated: 2024/09/06 19:19:19 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_array(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static size_t	count_words(const char *str, char sep)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		if (*str)
			count++;
		while (*str && *str != sep)
			str++;
	}
	return (count);
}

static int	fill_words_in_array(const char *str, char **array, char sep)
{
	size_t	i;
	size_t	start;

	i = 0;
	while (*str)
	{
		while (*str == sep)
			str++;
		start = 0;
		while (str[start] && str[start] != sep)
			start++;
		if (start > 0)
		{
			array[i] = ft_substr(str, 0, start);
			if (!array[i++])
			{
				free_array(array);
				return (0);
			}
		}
		str += start;
	}
	array[i] = NULL;
	return (1);
}

char	**ft_split(const char *str, char sep)
{
	char	**array;

	if (!str)
		return (NULL);
	array = (char **)malloc((count_words(str, sep) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	if (!fill_words_in_array(str, array, sep))
		return (NULL);
	return (array);
}
