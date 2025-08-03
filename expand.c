/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:57:43 by ataan             #+#    #+#             */
/*   Updated: 2025/08/03 21:20:27 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

static void	append_env_value(char **result, char *var_name, t_grand *grand)
{
	char	*var_value;
	char	*new_result;

	var_value = get_env_var(grand->env.envp, var_name);
	free(var_name);
	if (!var_value)
		var_value = "";
	new_result = ft_strjoin(*result, var_value);
	free(*result);
	*result = new_result;
}

static void	append_status(char **result, char *status)
{
	char	*new_result;

	new_result = ft_strjoin(*result, status);
	free(status);
	free(*result);
	*result = new_result;
}

static void	append_char(char **result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(*result, temp);
	free(*result);
	*result = new_result;
}

static void	handle_dollar(char **result, char **value_ptr, t_grand *grand)
{
	char	*value;
	char	*start;
	char	*var_name;
	char	*status;

	value = *value_ptr + 1;
	if (*value == '?')
	{
		status = ft_itoa(grand->chicken.status);
		append_status(result, status);
		*value_ptr = value + 1;
		return ;
	}
	start = value;
	while (*value && (ft_isalnum(*value) || *value == '_'))
		value++;
	if (value > start)
	{
		var_name = ft_substr(start, 0, value - start);
		append_env_value(result, var_name, grand);
	}
	else
		append_char(result, '$');
	*value_ptr = value;
}

/*
	handles qoutes and expansion
	result is malloced and should be freed later on
*/
char	*expand_variables(char *value, t_grand *grand)
{
	char	*result;

	result = ft_calloc(1, 1);
	while (*value)
	{
		if (*value == 39 && !grand->in_double)
		{
			value++;
			grand->in_single = !grand->in_single;
		}
		else if (*value == 34 && !grand->in_single)
		{
			value++;
			grand->in_double = !grand->in_double;
		}
		else if (*value == '$' && !grand->in_single)
			handle_dollar(&result, &value, grand);
		else
		{
			append_char(&result, *value);
			value++;
		}
	}
	return (result);
}
