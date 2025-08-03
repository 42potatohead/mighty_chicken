/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catagorize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:25:06 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:25:04 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

int	is_builtin(char *value)
{
	if (!ft_strncmp(value, "cd", ft_strlen(value)) || !ft_strncmp(value, "echo",
			ft_strlen(value)) || !ft_strncmp(value, "env", ft_strlen(value))
		|| !ft_strncmp(value, "exit", ft_strlen(value)) || !ft_strncmp(value,
			"export", ft_strlen(value)) || !ft_strncmp(value, "unset",
			ft_strlen(value)) || !ft_strncmp(value, "pwd", ft_strlen(value)))
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

int	catagory(char *value, t_grand *grand)
{
	if (is_builtin(value) == EXIT_SUCCESS && grand->token_counter == 1)
		return (TK_BLTN);
	else if (!ft_strncmp(value, "|", 1))
		return (TK_PIPE);
	else if (!ft_strncmp(value, ">", 1))
		return (TK_R_OUT);
	else if (!ft_strncmp(value, "<", 1))
		return (TK_R_IN);
	else if (!ft_strncmp(value, ">>", 2))
		return (TK_AOUT);
	else
		return (TK_CMD);
}
