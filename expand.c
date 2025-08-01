/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:57:43 by ataan             #+#    #+#             */
/*   Updated: 2025/07/31 14:23:37 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chicken.h"

static void	append_env_value(char **result, char *var_name, t_grand *grand)
{
	char	*var_value;
	char	*new_result;

	var_value = get_env_var(grand->env.envp, var_name);
	if (!var_value)
		var_value = ""; //clean_exit
	new_result = ft_strjoin(*result, var_value);
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

	value = *value_ptr + 1; // skip '$'
	if (*value == '?')
	{
		char *status = ft_itoa(grand->chicken.status);
		printf("handle $ status = %s\n", status);
		// append_env_value(result, status, grand);
		// append_char(result, '0');
		*result = ft_strjoin(*result, status);
		printf("append env value = %s\n", (*result));
		free(status);
		*value_ptr = value + 1; // advance past '?'
		return;
	}
	start = value;
	while (*value && (ft_isalnum(*value) || *value == '_'))
		value++;
	if (value > start)
	{
		var_name = ft_substr(start, 0, value - start);
		append_env_value(result, var_name, grand);
		free(var_name);
	}
	else
		append_char(result, '$');
	*value_ptr = value; // update caller's pointer
}

/*
	handles qoutes and expansion
	result is malloced and should be freed later on
*/
char *expand_variables(char *value, t_grand *grand)
{
	printf("expanding\n");
    char *result;

    result = ft_calloc(1, 1);
	while (*value) //   '"'
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
	printf("expander res = %s\n", result);
	return (result);
}
