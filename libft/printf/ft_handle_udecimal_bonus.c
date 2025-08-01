/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_integer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 15:14:33 by ataan             #+#    #+#             */
/*   Updated: 2024/09/08 17:26:15 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf_bonus.h"

static void	ft_calc_content(const unsigned int u, t_format_flags *flags,
		t_to_print *print_txt)
{
	free(print_txt->s);
	print_txt->s = ft_uitoa(u);
	if (u == 0 && flags->precision_set == 1 && flags->precision == 0)
		print_txt->s = ft_strdup("");
	print_txt->digits = ft_strlen(print_txt->s);
	if (flags->flag_zero && !(flags->flag_minus) && flags->precision_set == 0)
		print_txt->cpad = '0';
}

static void	ft_calc_len(t_format_flags *flags, t_to_print *print_txt)
{
	print_txt->tot_len = flags->field_width;
	if (flags->precision > print_txt->digits)
	{
		if (print_txt->tot_len < flags->precision
			+ (print_txt->prefix[0] != 'a'))
			print_txt->tot_len = flags->precision
				+ (print_txt->prefix[0] != 'a');
	}
	else if (print_txt->tot_len < print_txt->digits
		+ (print_txt->prefix[0] != 'a'))
		print_txt->tot_len = print_txt->digits + (print_txt->prefix[0] != 'a');
	print_txt->zeros = 0;
	if (flags->precision > print_txt->digits)
		print_txt->zeros = flags->precision - print_txt->digits;
	print_txt->pads = print_txt->tot_len - (print_txt->prefix[0] != 'a')
		- print_txt->zeros - print_txt->digits;
}

int	ft_handle_udecimal(va_list *args, t_format_flags *flags,
		t_to_print *print_txt)
{
	unsigned int	u;

	u = (unsigned int)va_arg(*args, unsigned int);
	ft_calc_content(u, flags, print_txt);
	ft_calc_len(flags, print_txt);
	if (flags->flag_minus)
		return (ft_print_left_adj(print_txt));
	else
		return (ft_print_right_adj(print_txt));
}
