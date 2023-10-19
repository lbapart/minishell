/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:05:10 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/09 16:38:46 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_parts(const char *str, char delimiter)
{
	int	count;
	int	is_in_part;

	count = 0;
	is_in_part = 0;
	while (*str)
	{
		if (*str == delimiter && is_in_part)
		{
			is_in_part = 0;
		}
		else if (!is_in_part && *str != delimiter)
		{
			is_in_part = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static int	next_part(const char *str, char delimiter, int offset, char *dest)
{
	int	i;

	i = 0;
	if (dest)
	{
		while (str[offset] && str[offset] != delimiter)
		{
			dest[i] = str[offset];
			i++;
			offset++;
		}
		dest[i] = '\0';
	}
	while (str[offset] && str[offset] == delimiter)
	{
		offset++;
	}
	return (offset);
}

static int	strlen_part(const char *str, char delimiter, int offset)
{
	int	length;

	length = 0;
	while (str[offset] && str[offset] != delimiter)
	{
		length++;
		offset++;
	}
	return (length);
}

static void	*free_null(char **result, int part_amount)
{
	int	i;

	i = 0;
	while (i < part_amount)
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		amount_parts;
	int		offset;
	int		current_part;
	int		length_part;
	char	**result;

	amount_parts = count_parts(s, c);
	result = (char **) malloc((amount_parts + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	result[amount_parts] = NULL;
	current_part = 0;
	offset = next_part(s, c, 0, NULL);
	while (current_part < amount_parts)
	{
		length_part = strlen_part(s, c, offset);
		result[current_part] = (char *) malloc(length_part + 1);
		if (!result[current_part])
			return (free_null(result, current_part));
		offset = next_part(s, c, offset, result[current_part]);
		current_part++;
	}
	return (result);
}
