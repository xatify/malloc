/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:23:12 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/26 23:23:52 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/**
 * we need to check if the given ptr
 * falls in the mapped region
 * 
 * @param ptr 
 */
void	free(void *ptr)
{
	t_block *b;
	t_zone	*z;

	if (!ptr)
		return ;
	b = (t_block *)((char *)ptr - sizeof(t_block));
	if (b->free == false)
	{
		z = get_zone_from_block(b);
		if (z->type == SMALL || z->type == TINY)
		{
			// no coalscing needed here 
			// we just set the block to free
			b->free = true;
		}
		else
		{
			// this block belongs to large zone
			// we need to do coalescing
			while (b)
			{
				b = coalesce_block(b);
				if (b->prev && b->prev->free)
					b = coalesce_block(b->prev);
				else
					break ;
			}
			b->free = true;
		}
		// if this is the last zone
		// we check if it's all free and return it
		// to the system.
		if (z->next == NULL)
		{
			if (free_zone(z))
			{
				if (z->prev)
				{
					z->prev->next = NULL;
				}
				else
				{
					// the first zone;
					// we need to set pbreak to NULL
					// return all the mapped memory to
					// the system
					pbreak = NULL;
				}
				if (munmap((void *)z, z->size) == -1)
					return ;
			}
		}
	}
}
