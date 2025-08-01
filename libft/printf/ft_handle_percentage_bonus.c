/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_percentage.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 15:14:33 by ataan             #+#    #+#             */
/*   Updated: 2024/09/08 17:26:15 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf_bonus.h"

int	ft_handle_percentage(va_list *args, t_format_flags *flags,
		t_to_print *print_txt)
{
	(void)args;
	if (flags->flag_zero == 0 || flags->flag_zero == 1)
		write(1, "%", 1);
	if (print_txt->s != NULL)
	{
		free(print_txt->s);
	}
	if (print_txt->prefix != NULL)
	{
		free(print_txt->prefix);
	}
	return (1);
}
