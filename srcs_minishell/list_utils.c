/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:03:29 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 17:19:09 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	lst_cmd_add_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	if (lst)
	{
		if (*lst)
		{
			last = lst_cmd_last(*lst);
			last->next = new;
		}
		else
			*lst = new;
	}
}

t_cmd	*lst_cmd_last(t_cmd *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	lst_redir_add_back(t_redirection **lst, t_redirection *new)
{
	t_redirection	*last;

	if (lst)
	{
		if (*lst)
		{
			last = lst_redir_last(*lst);
			last->next = new;
		}
		else
			*lst = new;
	}
}

t_redirection	*lst_redir_last(t_redirection *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
