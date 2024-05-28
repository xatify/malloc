/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:24:54 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/28 14:16:42 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*realloc(void *ptr, size_t size)
{
	t_block	*b;
	void	*p;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	b = (t_block *)((char *)ptr - sizeof(t_block));
	if (((((size - 1) >> 4) << 4) + 16) <= b->size)
		return (ptr);
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
