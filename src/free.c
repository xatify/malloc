/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:23:12 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/27 17:22:03 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	release_zone(t_zone *z);

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

	if (!ptr)
		return ;
	b = (t_block *)((char *)ptr - sizeof(t_block));
	if (b->free == false)
	{
		z = get_zone_from_block(b);
		if (z->type == LARGE)
		{
			while (b)
			{
				b = coalesce_block(b);
				if (b->prev && b->prev->free)
					b = coalesce_block(b->prev);
				else
					break ;
			}
		}
		b->free = true;
		release_zone(z);
	}
}

static void	release_zone(t_zone *z)
{
	if (z->next == NULL && free_zone(z))
	{
		if (z->prev)
			z->prev->next = NULL;
		else
			g_pbreak = NULL;
		munmap((void *)z, z->size);
	}
}
