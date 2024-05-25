
#include <stddef.h>
#include <stdbool.h>

/**
 * Zone type
 * 
 */
typedef enum e_zone_type
{
	TINY,
	SMALL,
	LARGE, 
}	t_zone_type;


/**
 * what does the size mean in this case]
 * the size that is used to call mmap to 
 * allocate this zone.
 * we will use the last 2 bit to indicate
 * the type of the the zone
 */
typedef struct zone
{
	t_zone	*next;
	t_zone	*prev;
	size_t	size;
	t_zone_type type;
}	t_zone;



/**
 * each block holds a double linked list
 * of this block
 * what does size mean in this case ?
 * the data size that can be/is requested by the application.
 * the LSB bit is used as a flag, to indicate if it's free or not
 */
typedef struct s_block
{
	t_block	*next;
	t_block *prev;
	size_t	size;
	bool free;
}	t_block;


/**
 * starting address of the heap
 */
void *pbreak = NULL;

/**
 * v1
 * 
 * @param size 
 * @return void* 
 */
void *v1malloc(size_t size)
{
	t_block *b;

	// check for size 0
	if (size == 0)
		return NULL;

	// try to find a free block
	b = get_free_block();
	if (!b)
		return NULL;
	// check if we can split the block
	b->size = size;
	b->free = false;
	b = try_split(b, size);

}