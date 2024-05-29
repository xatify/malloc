#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
	char *p = malloc(10);

	strcpy(p, "hello");
	printf("%p\n", p);
	p = realloc(p, 32);
	
	printf("%p %s\n", p, p);
	
	free(p);

	return (0);
}
