/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:22:09 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/26 23:42:12 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"



int main(int argc, char **argv)
{
	char *p;
	int *i;
	
	unsigned long *k;




	p = malloc(3);

	show_alloc_mem();
	i = malloc(2 * sizeof(int));

	show_alloc_mem();
	k = malloc(17 * sizeof(unsigned long));

	show_alloc_mem();
	free(i);

	show_alloc_mem();
	i = malloc(100 * sizeof(int));
	
	show_alloc_mem();
	
	show_alloc_mem();
	i = malloc(100 * sizeof(int));

	show_alloc_mem();	free(p);

	p = malloc(16 * sizeof(char));

	free (k);

	show_alloc_mem();

	p = malloc(1 * sizeof(char));
	show_alloc_mem();

	p = malloc(12 * sizeof(char));

	show_alloc_mem();

	k = malloc(1234 * sizeof(unsigned char));
	show_alloc_mem();

	free(p);
	show_alloc_mem();

	free(k);
	show_alloc_mem();
	
	free(i);

	show_alloc_mem();


	return (0);
}
