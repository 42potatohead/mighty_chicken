/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 15:14:33 by ataan             #+#    #+#             */
/*   Updated: 2024/11/04 15:56:59 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf_bonus.h"

static t_format_func	*init_format_handlers(void)
{
	static t_format_func	format_handlers[128] = {NULL};

	if (format_handlers['d'] == NULL)
	{
		format_handlers['d'] = ft_handle_integer;
		format_handlers['c'] = ft_handle_char;
		format_handlers['s'] = ft_handle_string;
		format_handlers['p'] = ft_handle_pointer;
		format_handlers['i'] = ft_handle_integer;
		format_handlers['u'] = ft_handle_udecimal;
		format_handlers['x'] = ft_handle_hex;
		format_handlers['X'] = ft_handle_hex_caps;
		format_handlers['%'] = ft_handle_percentage;
	}
	return (format_handlers);
}

static t_to_print	ft_initialize_print_txt(void)
{
	t_to_print	print_txt;

	print_txt.digits = 0;
	print_txt.zeros = 0;
	print_txt.tot_len = 0;
	print_txt.pads = 0;
	print_txt.s = ft_strdup("");
	print_txt.prefix = ft_strdup("a");
	print_txt.cpad = ' ';
	print_txt.cprint = 0;
	print_txt.print_char = 0;
	return (print_txt);
}

static void	no_handler(const char *fmt, int *ret)
{
	ft_putchar_fd(*fmt, 1);
	*(ret) += 1;
}

static int	ft_parse_and_print(va_list *args, const char *fmt,
		t_format_func *format_handlers)
{
	int				ret;
	t_format_flags	flags;
	t_format_func	handler;
	t_to_print		print_txt;

	ret = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			ft_parse_flags(&fmt, &flags);
			handler = format_handlers[(int)*fmt];
			print_txt = ft_initialize_print_txt();
			if (handler)
				ret += handler(args, &flags, &print_txt);
			else
				return (free(print_txt.s), free(print_txt.prefix), -1);
		}
		else
			no_handler(fmt, &ret);
		fmt++;
	}
	return (ret);
}

int	ft_printf(const char *fmt, ...)
{
	int				ret;
	va_list			args;
	t_format_func	*format_handlers;

	if (fmt == NULL)
		return (-1);
	va_start(args, fmt);
	format_handlers = init_format_handlers();
	ret = ft_parse_and_print(&args, fmt, format_handlers);
	va_end(args);
	return (ret);
}
