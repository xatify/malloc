/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:22:09 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/27 17:50:46 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"



int main(int argc, char **argv)
{
	
	/* silence the warning */
	(void)argc;
	(void)argv;


	char *p;
	int *i;
	
	unsigned long *k;

	p = malloc(3);

	i = malloc(2 * sizeof(int));

	k = malloc(17 * sizeof(unsigned long));

	free(i);

	i = malloc(100 * sizeof(int));	


	free(p);

	p = malloc(16 * sizeof(char));

	ft_memcpy(p, "hello there", ft_strlen("hello there") + 1);
	ft_write(p);
	
	p = realloc(p, 123);
	
	ft_write(p);

	free(p);

	free (k);


	p = malloc(1 * sizeof(char));

	free(p);

	p = malloc(12 * sizeof(char));


	k = malloc(1234 * sizeof(unsigned char));

	free(p);

	free(k);
	
	free(i);
	


	return (0);
}
