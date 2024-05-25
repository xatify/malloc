
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * starting address of the heap
 */
void *pbreak = NULL;

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
	t_ztype type;
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
 * align on 64-bit
*/
# define ALIGN8(x) ((((x - 1) >> 3) << 3) + 8)

/**
 * same as ALIGN8 but for page size
*/
size_t roundup(size_t size, size_t r)
{
	return (((size - 1) / r) * r) + r;
}

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


/**
 * given the size we return the zone type that
 * a block that holds size data belongs to
*/
t_ztype zone_type(size_t size)
{
	size_t as;

	as = ALIGN8(size);
	if (as <= TINYMAXSIZE)
		return (TINY);
	else if (as <= SMALLMAXSIZE)
		return (SMALL);
	return (LARGE);
}

/**
 * return the size to be request from the
 * system using mmap 
 * 
*/
size_t zone_size(t_ztype type, size_t size)
{
	int		ps;
	size_t	s;

	ps = getpagesize();
	if (type == TINY)
	{
		s = sizeof(t_zone) + (sizeof(t_block) + TINYMAXSIZE) * MINBLOCKNUM;
		return (roundup(s, ps));
	}
	else if (type == SMALL)
	{
		s = sizeof(t_zone) + (sizeof(t_block) + SMALLMAXSIZE) * MINBLOCKNUM;
		return (roundup(s, ps));
	}
	// we need to get the minimum number of pages
	// to hold that size
	return roundup(size, ps);
}

/**
 * create a new zone and attach it to last
 * last can be NULL, which means this is the
 * first zone that we will allocate
*/
t_zone	*create_new_zone(t_zone *last, size_t size)
{
	t_ztype		type;
	size_t		to_allocate;

	type = zone_type(size);
	if (type == TINY)
	{
		to_allocate = ((size - 1) / ps 
	}
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
			// we check if it has available free blocks
			// that can hold that data
			b = (t_block *)((char *)z + sizeof(t_zone));
			while (b)
			{
				if (b->size >= size && b->free == true)
				{
					// we found a free block
					return (b);
				}
				b = b->next;
			}
			// no block found
		}
		// we go to next zone
		z = z->next;
		last = z;
	}
	// no block found on any zone.
	// we need to create a new one
	// and attach it to last zone
	z = create_new_zone(last, size);

}

/**
 * given a free block and the size that it
 * needs to hold we try to split it
 * and return the right part of the split
*/
t_block *try_split(t_block *b, size_t size);


/**
 * given an allocated block
 * return the data pointer
*/
void *data_pointer(t_block *b);

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
	b = get_free_block(size);
	if (!b)
		return NULL;
	// check if we can split the block
	b = try_split(b, size);

	// set the size of the block
	// and label it as not free
	b->size = size;
	b->free = false;
	
	return data_pointer(b);
}