/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_pointer_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 15:14:33 by ataan             #+#    #+#             */
/*   Updated: 2024/10/28 15:12:21 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf_bonus.h"

static void	ft_calc_content_ptr(const uintptr_t ptr, t_format_flags *flags,
		t_to_print *print_txt)
{
	if (ptr != 0)
	{
		free(print_txt->prefix);
		print_txt->prefix = ft_strdup("0x");
	}
	if (ptr == 0)
	{
		free(print_txt->s);
		print_txt->s = ft_strdup("(nil)");
	}
	else
	{
		free(print_txt->s);
		print_txt->s = ft_ptrtoa(ptr);
	}
	print_txt->digits = ft_strlen(print_txt->s);
	if (flags->flag_zero && !(flags->flag_minus) && flags->precision_set == 0)
		print_txt->cpad = '0';
}

static void	ft_calc_len_ptr(t_format_flags *flags, t_to_print *print_txt)
{
	print_txt->tot_len = flags->field_width;
	if (flags->precision > print_txt->digits && print_txt->s[0] != '(')
	{
		if (print_txt->tot_len < flags->precision + 2)
			print_txt->tot_len = flags->precision + 2;
	}
	else if (print_txt->tot_len < print_txt->digits + 2
		&& print_txt->s[0] != '(')
		print_txt->tot_len = print_txt->digits + 2;
	else if (print_txt->tot_len < print_txt->digits)
		print_txt->tot_len = print_txt->digits;
	print_txt->zeros = 0;
	if (flags->precision > print_txt->digits && print_txt->s[0] != '(')
		print_txt->zeros = flags->precision - print_txt->digits;
	print_txt->pads = print_txt->tot_len - 2 * (print_txt->s[0] != '(')
		- print_txt->zeros - print_txt->digits;
}

int	ft_handle_pointer(va_list *args, t_format_flags *flags,
		t_to_print *print_txt)
{
	uintptr_t	ptr;

	ptr = (uintptr_t)va_arg(*args, void *);
	ft_calc_content_ptr(ptr, flags, print_txt);
	ft_calc_len_ptr(flags, print_txt);
	if (flags->flag_minus)
		return (ft_print_left_adj(print_txt));
	else
		return (ft_print_right_adj(print_txt));
}
