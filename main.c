
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

#include <sys/mman.h>


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
inline size_t roundup(size_t size, size_t r)
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
 * given an allocated block
 * return the address of the data
*/
inline void *data_pointer(t_block *b)
{
	return ((char *)b + sizeof(t_block));
}

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

	ps = getpagesize();
	if (type == TINY)
		size = sizeof(t_zone) + (sizeof(t_block) + TINYMAXSIZE) * MINBLOCKNUM;
	else if (type == SMALL)
		size = sizeof(t_zone) + (sizeof(t_block) + SMALLMAXSIZE) * MINBLOCKNUM;
	// we need to get the minimum number of pages
	// to hold that size
	return roundup(size, ps);
}


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
	void 		*zstart;

	zstart = last;
	type = zone_type(size);
	to_alloc = zone_size(type, size);
	if (last != NULL)
		zstart = last + ((t_zone *)last)->size;
	zstart = mmap(zstart, to_alloc, PROT_READ | PROT_WRITE, \
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zstart == MAP_FAILED)
		return NULL;
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
	z = create_new_zone(last, size);
	if (z == NULL)
		return (NULL);
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
 * v1
 * 
 * @param size 
 * @return void* 
 */
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
	
	return data_pointer(b);
}

/**
 * get the last mapped address
 * 
 * @return void* 
 */
void *last_maddress()
{
	t_zone *z;

	z = (t_zone *)pbreak;
	while (z && z->next)
		z = z->next;
	
	if (z)
		return ((char *)z + z->size);
	return NULL;
}

/**
 * @brief given a block 
 * return the zone it belongs to
 * @param b 
 * @return t_zone* 
 */
t_zone *get_zone_from_block(t_block *b)
{
	while (b && b->prev)
		b = b->prev;
	return (t_zone *)((char *)b - sizeof(t_zone));
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


/**
 * @brief check if the zone is all free
 * 
 * @param z 
 * @return true 
 * @return false
 */
bool free_zone(t_zone *z)
{
	t_block		*b;

	b = (t_block *)((char *)z + sizeof(t_zone));
	while (b)
	{
		if (b->free == false)
			return false;
		b = b->next;
	}
	return (true);
}

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
