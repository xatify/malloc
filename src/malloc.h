/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:02:23 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/26 23:44:18 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/mman.h>

# include "libft/libft.h"

/**
 * starting address of the heap
 */
extern void	*g_pbreak;

/**
 * Zone type
 * 
 */
typedef enum e_ztype
{
	TINY,
	SMALL,
	LARGE,
}	t_ztype;

/**
 * what does the size mean in this case
 * the size that is used to call mmap to 
 * allocate this zone.
 */
typedef struct s_zone
{
	struct s_zone	*next;
	struct s_zone	*prev;
	size_t			size;
	t_ztype			type;
}	t_zone;

/**
 * each block holds a double linked list
 * of this block
 * what does size mean in this case ?
 * the data size that can be/is requested by the application.
 */
typedef struct s_block
{
	struct s_block	*next;
	struct s_block	*prev;
	size_t			size;
	bool			free;
}	t_block;

/**
 * max data size in tiny/small zone
*/
# define TINYMAXSIZE	8
# define SMALLMAXSIZE	16

/**
 * minimum number of blocks in tiny
 * and small zones
*/
# define MINBLOCKNUM	100

t_ztype	zone_type(size_t size);
size_t	zone_size(t_ztype type, size_t size);
void	init_fblock(t_zone *z, size_t size);
t_zone	*init_new_zone(void *last, size_t size);
t_block	*find_fblock(t_zone *z, size_t sz);
t_block	*get_free_block(size_t size);
t_block	*try_split(t_block *b, size_t size);
t_zone	*get_zone_from_block(t_block *b);
t_block	*coalesce_block(t_block *b);
bool	free_zone(t_zone *z);

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);

#endif
