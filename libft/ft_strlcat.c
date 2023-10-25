/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:59:53 by ppfiel            #+#    #+#             */
/*   Updated: 2023/09/04 13:59:53 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char	*d;
	size_t	i;
	size_t	n;
	size_t	dlen;

	d = dst;
	i = 0;
	n = size;
	while (n-- != 0 && *d)
		d++;
	dlen = d - dst;
	n = size - dlen;
	if (n == 0)
		return (dlen + ft_strlen(src));
	while (src[i])
	{
		if (n != 1)
		{
			*d++ = src[i];
			n--;
		}
		i++;
	}
	*d = '\0';
	return (dlen + i);
}
