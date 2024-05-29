/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:23:12 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/28 14:53:21 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	release_zone(t_zone *z);

t_block	*findb(void *ptr)
{
	t_zone	*z;
	t_block	*b;

	if (ptr)
	{
		z = (t_zone *)g_pbreak;
		while (z)
		{
			if (ptr < (void *)z + z->size && ptr > (void *)z)
			{
				b = (void *)z + sizeof(t_zone);
				while (b)
				{
					if (ptr < (void *)b + sizeof(t_block) + b->size)
					{
						if ((void *)b < ptr)
							return (b);
					}
					b = b->next;
				}
			}
			z = z->next;
		}
	}
	return (NULL);
}

/**
* we find the block the data pointer 
 * belongs to, then we find the zone
 * if the zone is of type tyny or small
 * we just set the block as free, no
 * coalescing done here
 * otherwise, we merge adjacent free blocks.
 * if this is the last free zone,not at the head of
 * heap, we release it to the system. 
 * @param ptr 
 */

void	free(void *ptr)
{
	t_block	*b;
	t_zone	*z;
	t_zone	*tmp;

	if (!ptr)
		return ;
	b = findb(ptr);
	if (b && b->free == false)
	{	
		b = coalesce_block(b);
		b->free = true;
		z = (t_zone *)g_pbreak;
		while (z)
		{
			tmp = z->next;
			release_zone(z);
			z = tmp;
		}	
	}
#ifdef DEBUG
	show_alloc_mem();
#endif
}

void	release_zone(t_zone *z)
{
	if (free_zone(z))
	{
		if (z->prev)
			z->prev->next = z->next;
		if (z->next)
			z->next->prev = z->prev;
		if (z->prev == NULL)
			g_pbreak = NULL;
		munmap((void *)z, z->size);
	}
}
