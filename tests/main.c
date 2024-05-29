#include <stdlib.h>

#define LIMIT 100

int main()
{
	int **p = malloc(sizeof(int *) * LIMIT);

	for (int i=1; i < LIMIT; i++) {
		*(p + i) = malloc(sizeof(int) * i);
		if (*(p + i) == NULL)
				return (1);
		**(p + i) = 19999;
	}
	
	for (int i=0; i < LIMIT; i++)
		free(*(p + i));
	free(p);
	return (0);
}
