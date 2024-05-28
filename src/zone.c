/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:25:46 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/26 23:48:26 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/**
 * given the size we return the zone type that
 * a block that holds size data belongs to
*/
t_ztype	zone_type(size_t size)
{
	size_t	as;

	as = ((((size - 1) >> 3) << 3) + 8);
	if (as <= TINYMAXSIZE)
		return (TINY);
	else if (as <= SMALLMAXSIZE)
		return (SMALL);
	return (LARGE);
}

/**
 * return the size to be request from the
 * system using mmap 
 * some programs request the size of a page.
 * in this case the rounding will be to itself.
 * no more space for block and zone headers 
*/
size_t	zone_size(t_ztype type, size_t size)
{
	int	ps;

	ps = getpagesize();
	if (type == TINY)
		size = sizeof(t_zone) + (sizeof(t_block) + TINYMAXSIZE) * MINBLOCKNUM;
	else if (type == SMALL)
		size = sizeof(t_zone) + (sizeof(t_block) + SMALLMAXSIZE) * MINBLOCKNUM;
	else
		size = size + sizeof(t_block) + sizeof(t_zone);
	return (roundup(size, ps));
}

/**
 * create a new zone and attach it to last,
 * zstart can be NULL, which means this is the
 * first zone that we will allocate
 * zstart should be an address to the last
 * mapped zone
*/
t_zone	*init_new_zone(void *last, size_t size)
{
	t_ztype		type;
	size_t		to_alloc;
	t_zone		*zone;
	void		*zstart;

	zstart = last;
	type = zone_type(size);
	to_alloc = zone_size(type, size);
	if (last != NULL)
		zstart = last + ((t_zone *)last)->size;
	zstart = mmap(zstart, to_alloc, PROT_READ | PROT_WRITE, \
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zstart == MAP_FAILED)
		return (NULL);
	if (g_pbreak == NULL)
		g_pbreak = zstart;
	zone = (t_zone *)zstart;
	zone->next = NULL;
	zone->prev = last;
	if (last != NULL)
		((t_zone *)last)->next = zone;
	zone->size = to_alloc;
	zone->type = type;
	init_fblock(zone, to_alloc);
	return (zone);
}

/**
 * @brief check if the zone is all free
 * 
 * @param z 
 * @return true 
 * @return false
 */
bool	free_zone(t_zone *z)
{
	t_block		*b;

	b = (t_block *)((char *)z + sizeof(t_zone));
	while (b)
	{
		if (b->free == false)
			return (false);
		b = b->next;
	}
	return (true);
}

/**
 * @brief given a block 
 * return the zone it belongs to
 * @param b 
 * @return t_zone* 
 */
t_zone	*get_zone_from_block(t_block *b)
{
	while (b && b->prev)
		b = b->prev;
	return ((t_zone *)((char *)b - sizeof(t_zone)));
}
