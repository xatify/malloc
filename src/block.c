/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:27:06 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/26 23:46:57 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/**
 * initiatize a free block that will span the
 * entire zone
 * @param z new created zone
 * @param size the allocated region for that zone
 */
void init_fblock(t_zone *z, size_t size)
{
	t_block *b;

	b = (t_block *)((char *)z + sizeof(t_zone));
	b->next = NULL;
	b->prev = NULL;
	b->free = true;
	b->size = size - sizeof(t_zone) - sizeof(t_block);
}

/**
 * @brief iterate over a zone to find a free block
 * 
 * @param z zone
 * @param sz the size block must be >= sz 
 * @return t_block* pointer to the free zone
 */
t_block *find_fblock(t_zone *z, size_t sz)
{
	t_block		*b;

	b = (t_block *)((char *)z + sizeof(t_zone));
	while (b)
	{
		if ((b->free == true) && (b->size >= ALIGN8(sz)))
			break ;
		b = b->next;
	}
	return (b);
}

/**
 * this is the main part of malloc
 * it need to find the zone, then a free block 
 * inside that zone that can hold that size
 * if a zone could not be found, we create one
 * and initiate a free block inside it.
 * and return it.
 * this function uses first fit policy
 * we return the fist zone, then block that can
 * satisfy this requested size.
*/
t_block *get_free_block(size_t size)
{
	t_zone	*z;
	t_zone	*last;
	t_block *b;

	z = (t_zone *)pbreak;
	b = NULL;
	last = z;
	while (z)
	{
		// we check if this is the right zone given the size
		if (z->type == zone_type(size))
		{
			b = find_fblock(z, size);
			if (b)
				return (b);
			// no block found
		}
		// we go to next zone
		z = z->next;
		last = z;
	}
	// no block found on any zone.
	// we need to create a new one
	// and attach it to last zone
	z = init_new_zone(last, size);
	if (z == NULL)
		return (NULL);
	if (pbreak)
	// this is guaranted to succed
	return (find_fblock(z, size));
}

/**
 * given a free block and the size that it
 * needs to hold we try to split it
 * and return the right part of the split
*/
t_block *try_split(t_block *b, size_t size)
{
	t_block		*newb;

	if (b->size >= (ALIGN8(size) + sizeof(t_block)))
	{
		newb = (t_block *)((char *)b + ALIGN8(size));
		newb->size = b->size - (ALIGN8(size) + sizeof(t_block));
		newb->free = true;
		newb->prev = b;
		b->next = newb;
		newb->next = b->next;
		if (b->next)
			b->next->prev = newb;
		return (newb);
	}
	return (b);
}

/**
 * @brief merge a block with it's next block
 * if it's free too
 * 
 * @param b block
 * @return t_block *
 */
t_block	*coalesce_block(t_block *b)
{
	t_block *tmp;

	if (b && b->next && b->next->free)
	{
		tmp = b->next;
		b->next = b->next->next;
		if (tmp->next)
			tmp->next->prev = b;
		b->size = b->size + tmp->size + sizeof(t_block);
		b->free = true;
	}
	return (b);
}
