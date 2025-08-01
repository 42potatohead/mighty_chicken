/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_left_right_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:10:21 by ataan             #+#    #+#             */
/*   Updated: 2024/10/28 15:10:35 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_print_left_adj(t_to_print *print_txt)
{
	if (print_txt->prefix[0] != 'a')
		ft_putstr_fd(print_txt->prefix, 1);
	while (print_txt->zeros-- > 0)
		ft_putchar_fd('0', 1);
	if (print_txt->print_char != 0)
	{
		ft_putchar_fd(print_txt->cprint, 1);
	}
	else
		ft_putstr_fd(print_txt->s, 1);
	while (print_txt->pads-- > 0)
		ft_putchar_fd(print_txt->cpad, 1);
	if (print_txt->s != NULL)
	{
		free(print_txt->s);
	}
	if (print_txt->prefix != NULL)
	{
		free(print_txt->prefix);
	}
	return (print_txt->tot_len);
}

int	ft_print_right_adj(t_to_print *print_txt)
{
	if (print_txt->cpad == ' ')
	{
		while (print_txt->pads-- > 0)
			ft_putchar_fd(print_txt->cpad, 1);
	}
	if (print_txt->prefix[0] != 'a')
		ft_putstr_fd(print_txt->prefix, 1);
	if (print_txt->cpad == '0')
	{
		while (print_txt->pads-- > 0)
			ft_putchar_fd(print_txt->cpad, 1);
	}
	while (print_txt->zeros-- > 0)
		ft_putchar_fd('0', 1);
	if (print_txt->print_char != 0)
		ft_putchar_fd(print_txt->cprint, 1);
	else
		ft_putstr_fd(print_txt->s, 1);
	if (print_txt->s != NULL)
		free(print_txt->s);
	if (print_txt->prefix != NULL)
		free(print_txt->prefix);
	return (print_txt->tot_len);
}
