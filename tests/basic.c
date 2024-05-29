#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	char *p;

	p = malloc(sizeof(char) * 20);
	
	strcpy(p, "hello there\n");

	printf("%s\n", p);
	printf("pointer 0x%p\n", p);

	free(p);
	return 0;
}
