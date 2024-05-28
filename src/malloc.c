/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:24:12 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/27 17:23:11 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/**
 * we find a free block, we acquire a zone
 * if no block is available
 * the first block in a zone, takes the entire zone.
 * we split the block if it has more available space.
 * @param size 
 * @return void* data pointer
 */

void	*g_pbreak = NULL;

void	*malloc(size_t size)
{
	t_block	*b;

	if (size == 0)
		return (NULL);
	b = get_free_block(size);
	if (!b)
		return (NULL);
	b = try_split(b, size);
	b->free = false;
	return ((char *)b + sizeof(t_block));
}
