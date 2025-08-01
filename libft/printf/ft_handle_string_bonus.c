/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_string_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 15:14:33 by ataan             #+#    #+#             */
/*   Updated: 2024/10/28 15:11:51 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf_bonus.h"

static void	ft_calc_content(const char *s, t_format_flags *flags,
		t_to_print *print_txt)
{
	free(print_txt->s);
	if (s == NULL)
		print_txt->s = ft_strdup("(null)");
	else
		print_txt->s = ft_strdup(s);
	if (s == NULL && flags->precision_set == 1 && flags->precision < 6)
		print_txt->s = ft_strdup("");
	if (flags->precision_set && s != NULL)
	{
		if (flags->precision == 0)
		{
			free(print_txt->s);
			print_txt->s = ft_strdup("");
		}
		else
		{
			free(print_txt->s);
			print_txt->s = ft_substr(s, 0, flags->precision);
		}
	}
	print_txt->digits = ft_strlen(print_txt->s);
}

static void	ft_calc_len(t_format_flags *flags, t_to_print *print_txt)
{
	print_txt->tot_len = flags->field_width;
	if (print_txt->tot_len < print_txt->digits)
		print_txt->tot_len = print_txt->digits;
	print_txt->zeros = 0;
	print_txt->pads = print_txt->tot_len - print_txt->digits;
}

int	ft_handle_string(va_list *args, t_format_flags *flags,
		t_to_print *print_txt)
{
	char	*s;

	s = va_arg(*args, char *);
	ft_calc_content(s, flags, print_txt);
	ft_calc_len(flags, print_txt);
	if (flags->flag_minus)
		return (ft_print_left_adj(print_txt));
	else
		return (ft_print_right_adj(print_txt));
}
