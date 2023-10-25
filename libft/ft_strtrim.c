/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:46:29 by ppfiel            #+#    #+#             */
/*   Updated: 2023/09/06 09:16:17 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start_pos;
	size_t	end_pos;
	size_t	i;
	char	*str;

	start_pos = 0;
	while (s1[start_pos] && ft_strchr(set, s1[start_pos]) != NULL)
		start_pos++;
	end_pos = ft_strlen(s1);
	while (end_pos > start_pos && ft_strchr(set, s1[end_pos - 1]) != NULL)
		end_pos--;
	str = malloc(end_pos - start_pos + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (start_pos < end_pos)
	{
		str[i] = s1[start_pos];
		i++;
		start_pos++;
	}
	str[i] = '\0';
	return (str);
}
