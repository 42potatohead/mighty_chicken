/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 15:14:33 by ataan             #+#    #+#             */
/*   Updated: 2024/09/08 17:26:15 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf_bonus.h"

static void	ft_calc_content(const int c, t_to_print *print_txt)
{
	print_txt->print_char = 1;
	print_txt->cprint = c;
}

static void	ft_calc_len(t_format_flags *flags, t_to_print *print_txt)
{
	if (flags->field_width > 0)
		print_txt->tot_len = flags->field_width;
	else
		print_txt->tot_len = 1;
	print_txt->pads = print_txt->tot_len - 1;
	print_txt->zeros = 0;
	print_txt->digits = 0;
}

int	ft_handle_char(va_list *args, t_format_flags *flags, t_to_print *print_txt)
{
	int	c;

	c = va_arg(*args, int);
	ft_calc_content(c, print_txt);
	ft_calc_len(flags, print_txt);
	if (flags->flag_minus)
		return (ft_print_left_adj(print_txt));
	else
		return (ft_print_right_adj(print_txt));
}
