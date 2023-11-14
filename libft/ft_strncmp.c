/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:59:59 by ppfiel            #+#    #+#             */
/*   Updated: 2023/09/04 13:59:59 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < (n - 1) && s1[i] && s2[i])
	{
		if (((unsigned char) s1[i]) != ((unsigned char) s2[i]))
			return (((unsigned char) s1[i]) - ((unsigned char) s2[i]));
		i++;
	}
	return (((unsigned char) s1[i]) - ((unsigned char) s2[i]));
}
