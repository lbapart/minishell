/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:35:23 by ppfiel            #+#    #+#             */
/*   Updated: 2023/09/06 15:15:25 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	calc_length(char const *s, unsigned int start, size_t len)
{
	size_t			s_len;
	unsigned int	reduced_length;

	s_len = ft_strlen(s);
	if (s_len == 0 || start >= s_len)
		return (0);
	reduced_length = s_len - start;
	if (reduced_length > len)
		return (len);
	return (reduced_length);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	j;

	substr = ft_calloc(calc_length(s, start, len) + 1, 1);
	if (!substr)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && j < len)
	{
		if (i >= start)
		{
			substr[j] = s[i];
			j++;
		}
		i++;
	}
	substr[j] = '\0';
	return (substr);
}
