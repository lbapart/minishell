/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:39:19 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 20:40:25 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcat(char *dest, const char *src)
{
	size_t i;
	size_t j;

	i = 0;
	j = ft_strlen(dest);
	while (src && src[i])
		dest[j++] = src[i++];
	dest[j] = '\0';
}
