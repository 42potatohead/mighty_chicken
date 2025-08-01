/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 19:26:20 by ataan             #+#    #+#             */
/*   Updated: 2024/09/06 19:26:21 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	find_len_num(int n, size_t *len, unsigned int *num)
{
	int	temp;

	if (n < 0)
	{
		*len = 1;
		*num = -n;
	}
	else
	{
		*len = 0;
		*num = n;
	}
	temp = *num;
	while (temp != 0)
	{
		temp /= 10;
		(*len)++;
	}
}

char	*ft_itoa(int n)
{
	size_t			len;
	unsigned int	num;
	char			*p;

	if (n == 0)
		return (ft_strdup("0"));
	find_len_num(n, &len, &num);
	p = (char *)malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	p[len] = '\0';
	if (n < 0)
		p[0] = '-';
	while (num != 0)
	{
		p[--len] = '0' + (num % 10);
		num /= 10;
	}
	return (p);
}
