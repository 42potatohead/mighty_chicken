/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_integer_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 15:14:33 by ataan             #+#    #+#             */
/*   Updated: 2024/09/17 12:28:14 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf_bonus.h"

static void	ft_calc_s(const int i, t_format_flags *flags, t_to_print *print_txt)
{
	free(print_txt->s);
	if (i == 0 && flags->precision_set == 1 && flags->precision == 0)
		print_txt->s = ft_strdup("");
	else if (i > 0)
		print_txt->s = ft_itoa(i);
	else if (i == -2147483648)
		print_txt->s = ft_strdup("2147483648");
	else
		print_txt->s = ft_itoa(-i);
}

static void	ft_calc_content(const int i, t_format_flags *flags,
		t_to_print *print_txt)
{
	ft_calc_s(i, flags, print_txt);
	print_txt->digits = ft_strlen(print_txt->s);
	if (flags->flag_zero && !(flags->flag_minus) && flags->precision_set == 0)
		print_txt->cpad = '0';
	else
		print_txt->cpad = ' ';
	if (i < 0)
	{
		free(print_txt->prefix);
		print_txt->prefix = ft_strdup("-");
	}
	else if (flags->flag_plus && i >= 0)
	{
		free(print_txt->prefix);
		print_txt->prefix = ft_strdup("+");
	}
	else if (flags->flag_space && i >= 0)
	{
		free(print_txt->prefix);
		print_txt->prefix = ft_strdup(" ");
	}
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

int	ft_handle_integer(va_list *args, t_format_flags *flags,
		t_to_print *print_txt)
{
	int	i;

	i = va_arg(*args, int);
	ft_calc_content(i, flags, print_txt);
	ft_calc_len(flags, print_txt);
	if (flags->flag_minus)
		return (ft_print_left_adj(print_txt));
	else
		return (ft_print_right_adj(print_txt));
}
