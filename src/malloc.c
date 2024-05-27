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
 * v1
 * 
 * @param size 
 * @return void* 
 */

void *pbreak = NULL;

void *malloc(size_t size)
{
	t_block *b;

	// check for size 0
	if (size == 0)
		return NULL;

	// try to find a free block
	b = get_free_block(size);
	if (!b)
		return NULL;
	// check if we can split the block
	b = try_split(b, size);
	b->free = false;

#ifdef DEBUG
	show_alloc_mem();
#endif	
	return DATA(b);
}
