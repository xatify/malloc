/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:24:54 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/27 18:18:16 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void *realloc(void *ptr, size_t size)
{
	t_block	*b;
	void	*p;

	// special cases
	if (ptr == NULL)
		return (malloc(size));
	if (size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	// if the block can hold the 
	// the size we return it as is no
	b = (t_block *)((char *)ptr - sizeof(t_block));
	if (ALIGN8(size) <= b->size)
		return (ptr);
	
	// otherwise we need to find a new block to
	// hold size data
	// copy the old data there
	p = malloc(size);
	if (p == NULL)
		return (NULL);
	ft_memcpy(p, ptr, b->size);
	free(ptr);
#ifdef DEBUG
	show_alloc_mem();
#endif
	return (p);
}
