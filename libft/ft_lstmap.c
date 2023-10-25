/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 14:15:15 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/13 09:55:31 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*mapped_lst;
	t_list	*temp;
	void	*temp_content;

	if (!lst || !f || !del)
		return (NULL);
	mapped_lst = NULL;
	while (lst)
	{
		temp_content = f(lst->content);
		temp = ft_lstnew(temp_content);
		if (!temp)
		{
			free(temp_content);
			ft_lstclear(&mapped_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&mapped_lst, temp);
		lst = lst->next;
	}
	return (mapped_lst);
}
