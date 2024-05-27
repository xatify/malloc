/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:39:36 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/27 16:55:11 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"


static const char* ZTYPES[] = {
	"TINY",
	"SMALL",
	"LARGE",
};

static char *print_base(void *ptr, char *buf, int s, int base)
{
	size_t sptr = (size_t )ptr;
	int			c;
	
	buf[s] = '\0';
	s--;
	buf[s] = '0';
	while (sptr)
	{
		c = sptr % base;
		c = (c >= 10) ? 'A' + (c - 10): c + '0';
		buf[s] = c;
		sptr = sptr / base;
		if (sptr)
			s--;
	}
	if (base == 16)
	{
		buf[--s] = 'x';
		buf[--s] = '0';
	}
	return (buf + s);
}


void print_zone(t_zone *z)
{
	char	buf[16 + 1];
	t_block	*b;
	
	ft_write("\n");
	ft_write(ZTYPES[z->type]);
	ft_write(" : ");
	ft_write(print_base(z, buf, 16, 16));
	ft_write("\n");

	b = (t_block *)((char *)z + sizeof(t_zone));
	while(b)
	{
		if (b->free == false)
		{
			ft_write(print_base(DATA(b), buf, 16, 16));
			ft_write(" - ");
			ft_write(print_base(DATA(b) + b->size, buf, 16, 16));
			ft_write(" : ");
			ft_write(print_base((unsigned char *)(b->size), buf, 16, 10));
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
	ft_write("----------------------------\n");
	while (z)
	{
		print_zone(z);
		z = z->next;
	}
}
