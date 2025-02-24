/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chicken.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:02:19 by ataan             #+#    #+#             */
/*   Updated: 2025/02/20 14:02:19 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHICKEN_H
# define CHICKEN_H

#include <stdio.h>
#include "./libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_chicken
{
    char *input;
    char *builtins[8];
    char **tokens;
} t_chicken;

typedef struct s_grand
{
    t_chicken chicken;
} t_grand;

# endif
