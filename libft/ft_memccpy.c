/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:59:36 by ppfiel            #+#    #+#             */
/*   Updated: 2023/09/04 13:59:36 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	byte;

	i = 0;
	while (i < n)
	{
		byte = ((unsigned char *) src)[i];
		((unsigned char *) dest)[i] = byte;
		i++;
		if (byte == ((unsigned char)c))
			return (dest + i);
	}
	return (NULL);
}
