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

static char	*print_base(void *ptr, char *buf, int s, int base)
{
	size_t		sptr;
	int			c;

	sptr = (size_t)ptr;
	buf[s--] = '\0';
	buf[s] = '0';
	while (sptr)
	{
		c = sptr % base;
		if (c >= 10)
			c = 'A' + (c - 10);
		else
			c = c + '0';
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

static void	print_zone_header(t_zone *z)
{
	char				buf[16 + 1];
	static const char	*ztypes[] = {
		"TINY",
		"SMALL",
		"LARGE"
	};

	ft_write(ztypes[z->type]);
	ft_write(" : ");
	ft_write(print_base(z, buf, 16, 16));
	ft_write("\n");
}

void	print_zone(t_zone *z)
{
	char				buf[16 + 1];
	t_block				*b;
	void				*dptr;

	print_zone_header(z);
	b = (t_block *)((char *)z + sizeof(t_zone));
	while (b)
	{
		dptr = (char *)b + sizeof(t_block);
		if (b->free == false)
		{
			ft_write(print_base(dptr, buf, 16, 16));
			ft_write(" - ");
			ft_write(print_base(dptr + b->size, buf, 16, 16));
			ft_write(" : ");
			ft_write(print_base((unsigned char *)(b->size), buf, 16, 10));
			ft_write(" bytes\n");
		}
		b = b->next;
	}
}

/**
 * show allocated memory
*/
void	show_alloc_mem(void)
{
	t_zone	*z;

	z = (t_zone *)g_pbreak;
	ft_write("----------------------------\n");
	while (z)
	{
		print_zone(z);
		z = z->next;
	}
}
