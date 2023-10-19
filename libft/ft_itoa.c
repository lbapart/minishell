/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 08:40:42 by ppfiel            #+#    #+#             */
/*   Updated: 2023/09/06 11:52:44 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*convert_chars(char *chars, int i, int n)
{
	char	*str;
	size_t	j;

	str = malloc(i + 1 + (n < 0));
	if (!str)
		return (NULL);
	j = 0;
	if (n < 0)
	{
		str[0] = '-';
		j++;
	}
	while (--i >= 0)
		str[j++] = chars[i];
	str[j] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char		chars[12];
	long long	num;
	int			i;

	num = n;
	i = 0;
	if (num < 0)
		num = -num;
	else if (num == 0)
	{
		chars[0] = '0';
		i++;
	}
	while (num > 0)
	{
		chars[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	return (convert_chars(chars, i, n));
}
