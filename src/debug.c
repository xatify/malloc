/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:39:36 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/27 00:45:29 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"


const static char* ZTYPES[] = {
	"TINY",
	"SMALL",
	"LARGE",
};


static char *hexa_address(void *ptr, char *buf, int s)
{
	size_t sptr = (size_t )ptr;
	int			c;
	
	buf[s] = '\0';
	s--;
	buf[s] = '0';
	while (sptr)
	{
		c = sptr % 16;
		c = (c >= 10) ? 'A' + (16 - c): c + '0';
		buf[s] = c;
		sptr = sptr / 16;
		if (sptr)
			s--;
	}
	s--;
	buf[--s] = 'x';
	buf[--s] = '0';
	return (buf + s);
}


void print_zone(t_zone *z)
{
	char buf[16 + 1];
	
	ft_write(ZTYPES[z->type]);
	ft_write(" : ");
	ft_write(hexa_address(z, buf, 16));
	ft_write("\n");

	t_block *b;

	b = (t_block *)((char *)z + sizeof(t_zone));
	while(b)
	{
		if (b->free == false)
		{
			ft_write(hexa_address(DATA(b), buf, 16));
			ft_write(" - ");
			ft_write(hexa_address(DATA(b) + b->size, buf, 16));
			ft_write(" : ");
			ft_write(iprint(b->size. buf, 16));
			ft_write(" bytes\n");
		}
		b = b->next;
	}
	
}


/**
 * show allocate memory
*/
void show_alloc_mem()
{
	t_zone	*z;

	z = (t_zone *)pbreak;
	while (z)
	{
		print_zone(z);
		z = z->next;
	}
}